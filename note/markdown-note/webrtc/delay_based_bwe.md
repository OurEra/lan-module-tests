### InterArrival 
  在基于延迟做带宽估计的环节中, InterArrival 首先计算每组数据包的发送时间差, 和接收到的时间差;
用于判断在传输过程中,是否发生了延时排队导致接收时间差变大,或者延时状态变好让一段时间内的数据包
接收时间变小.

```c++
bool InterArrival::ComputeDeltas(uint32_t timestamp,
                                 int64_t arrival_time_ms,
                                 int64_t system_time_ms,
                                 size_t packet_size,
                                 uint32_t* timestamp_delta,
                                 int64_t* arrival_time_delta_ms,
                                 int* packet_size_delta) {
      ...
      *timestamp_delta =
          current_timestamp_group_.timestamp - prev_timestamp_group_.timestamp;
      *arrival_time_delta_ms = current_timestamp_group_.complete_time_ms -
                               prev_timestamp_group_.complete_time_ms;
      ...
}
```
  InterArrival 根据传入的数据包的发送时间, 和到达时间; 当数据包时间超过 **kTimestampGroupLengthMs = 5** 为一组记录,
然后计算两个时间差.

### TrendlineEstimator
#### 1 计算窗口内斜率
  拿到 InterArrival 计算出来的时间差, TrendlineEstimator 然后根据一段窗口时间内的数据, 计算时间差的趋势, 来判定是否发生了延时排队或者
延时恢复.
  首先, 将发送和接收时间差之差作为 y 轴数据, 将接收时间作为 x 轴数据:
```c++
    ...
    const double delta_ms = recv_delta_ms - send_delta_ms;
    if (first_arrival_time_ms_ == -1)
      first_arrival_time_ms_ = arrival_time_ms;

    // Exponential backoff filter.
    accumulated_delay_ += delta_ms;
    smoothed_delay_ = smoothing_coef_ * smoothed_delay_ +
                      (1 - smoothing_coef_) * accumulated_delay_;

    // Simple linear regression.
    delay_hist_.push_back(std::make_pair(
        static_cast<double>(arrival_time_ms - first_arrival_time_ms_),
        smoothed_delay_));
    ...
```
当数据满足 **kDefaultTrendlineWindowSize = 20** 时, 计算一个 x,y 的斜率值:
  当 trend == 0 时, 表示一组数据包的发送和接收间隔是一致的; 即网络延时没有变化;
  当 trend > 0 时, 表示一组数据包的接收间隔相对于发送间隔在增长; 即网络延时在变差;
  当 trend < 0 时, 表示一组数据包的接收间隔相对于发送间隔在降低; 即网络延时在变好, 一些堆积的数据包快速到达接收端;

#### 2 平滑计算 BandwidthUsage
  为了方便计算，计算出来的斜率值首先乘上了一个系数:
```c++
  const double modified_trend =
      std::min(num_of_deltas_, kMinNumDeltas) * trend * threshold_gain_;
```
这个系数，将与一个不断更新的缓存系数比较 **threshold_**, 计算出当前带宽的使用情况; 其中
**threshold_** 的初值为 12.5; 根据其他系数的值，可以推算出 **trend** 的斜率超过大概 0.05 则表示 **kBwOverusing**;

  当 modified_trend > threshold_ 时，表示**kBwOverusing**; 即当前窗口的 trend 增加的变化超过了之前的门限值;
  当 modified_trend < -threshold_ 时，表示**kBwUnderusing**; 即当前窗口的 trend 降低的程度超过了之前的门限值;
  其他，则表示**kBwNormal**.

**threshold_** 的更新由下面逻辑完成:
```c++
  const double k = fabs(modified_trend) < threshold_ ? k_down_ : k_up_;
  const int64_t kMaxTimeDeltaMs = 100;
  int64_t time_delta_ms = std::min(now_ms - last_update_ms_, kMaxTimeDeltaMs);
  threshold_ += k * (fabs(modified_trend) - threshold_) * time_delta_ms;
  threshold_ = rtc::SafeClamp(threshold_, 6.f, 600.f);
```
其中, **k_up_ = 0.0087**, **k_down_ = 0.039**;

  若通过斜率 **trend** 计算出来的 **modified_trend** 一直变化比较小，即 **< threshold_**, 则 **threshold_** 将会以一个 **k_down_**
系数逐步降低 **threshold_** 值, 这个降低的速度和 **modified_trend** 与 **threshold_** 相对差正相关, 即让 **threshold_** 快速和
当前的 **trend** 匹配; 最低情况，**threshold_** 为 6, 即 **trend** 斜率大概为 0.025 则被判断为 **kBwOverusing**; 若 **modified_trend**
超过了 **threshold_**, 则 **threshold_** 会以系数 **k_up_** 增加, 同样增加的速度和相关差值正相关;
  这里的逻辑，可以描述为一定窗口内的 **modified_trend** 和 **threshold_** 比较的过程，**threshold_** 由系数控制和前一个 **modified_trend**
逼近; 使得既可以感知较低的斜率变化，又可以适应高变化时下的持续性变动．


### AimdRateControl
  当做带宽的升降时，另外一个模块 LinkCapacityEstimator 用于简单限制带宽升高的方式;
> deviation_kbps_ 取值 0.4 - 2.5; 当前的带宽吞吐值和上次偏差值越大时，这个值就会上升
>
> const double norm = std::max(estimate_kbps_.value(), 1.0);
> double error_kbps = estimate_kbps_.value() - sample_kbps;
> deviation_kbps_ =
>     (1 - alpha) * deviation_kbps_ + alpha * error_kbps * error_kbps / norm;
> // 0.4 ~= 14 kbit/s at 500 kbit/s
> // 2.5f ~= 35 kbit/s at 500 kbit/s
> deviation_kbps_ = rtc::SafeClamp(deviation_kbps_, 0.4f, 2.5f);
>
> 函数 UpperBound() 返回的是
> DataRate::kbps(estimate_kbps_.value() +
>    3 * deviation_estimate_kbps());
> 目前 webrtc 并没有使用这个具体值，而是判断 LinkCapacityEstimator 如果有有效值来决定带宽升高方式

AimdRateControl 的动作主要在 Update() 中完成:
#### 1 更新状态
根据 RateControlInput.bw_state 中传入的 BandwidthUsage 做状态的更新:
```c++
  switch (input.bw_state) {
    case BandwidthUsage::kBwNormal:
      if (rate_control_state_ == kRcHold) {
        time_last_bitrate_change_ = at_time;
        rate_control_state_ = kRcIncrease;
      }
      break;
    case BandwidthUsage::kBwOverusing:
      if (rate_control_state_ != kRcDecrease) {
        rate_control_state_ = kRcDecrease;
      }
      break;
    case BandwidthUsage::kBwUnderusing:
      rate_control_state_ = kRcHold;
      break;
    default:
      assert(false);
  }
```
如果 kBwOverusing 进入带宽降低；如果 kBwUnderusing 先进入带宽保持，如果后续进入 kBwNormal, 则进入带宽增加;

#### 1 带宽增减
根据状态更新的情况，具体计算带宽增加或者降低的方式: 
当增加带宽时，分为两个情况，如果 LinkCapacityEstimator 有有效值，进行 AdditiveRateIncrease():
```c++
  double time_period_seconds = (at_time - last_time).seconds<double>();
  double data_rate_increase_bps =
      GetNearMaxIncreaseRateBpsPerSecond() * time_period_seconds;
```
其中, GetNearMaxIncreaseRateBpsPerSecond() 的值是计算一个数据包的平均值，然后乘以 rtt 的倒数，即得到一秒内最多发送的数据;
```c++
  double increase_rate_bps_per_second =
      (avg_packet_size / response_time).bps<double>();
```
如果 LinkCapacityEstimator 没有有效值，则当前初始状态，可以进行 MultiplicativeRateIncrease():
```c++
  double alpha = 1.08;
  if (last_time.IsFinite()) {
    auto time_since_last_update = at_time - last_time;
    alpha = pow(alpha, std::min(time_since_last_update.seconds<double>(), 1.0));
  }
  DataRate multiplicative_increase =
      std::max(current_bitrate * (alpha - 1.0), DataRate::bps(1000));
```
即最低每次按 8% 来增加;

当降低带宽时, 抛开其他没有实现的逻辑; 实际上就是按照一个 ** kDefaultBackoffFactor = 0.85 ** 系数来降低:
```c++
if (estimated_throughput > low_throughput_threshold_) {
  // Set bit rate to something slightly lower than the measured throughput
  // to get rid of any self-induced delay.
  new_bitrate = estimated_throughput * beta_;
  if (new_bitrate > current_bitrate_) {
    // Avoid increasing the rate when over-using.
    if (link_capacity_.has_estimate()) {
      new_bitrate = beta_ * link_capacity_.estimate();
    }
  }
}
```

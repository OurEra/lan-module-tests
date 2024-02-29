
  AcknowledgedBitrateEstimator 通过 IncomingPacketFeedbackVector() 接收 feedback, 并结合 SetAlrEndedTimeMs()
状态, 来返回码率估计 bitrate().

  具体估计位于 BitrateEstimator, 其中  Update() 用于通过UpdateWindow() 来计算当前窗口时间内的带宽观测值, 并结合上一次的预测值来
做出当前带宽估计.
```c++
// 这其实对应的是一个卡尔曼率滤波的后验期望的更新过程
// 后验期望:exp[k]+ = exp[k]ˉ + k*(y[k] - h* exp[k]ˉ)
// 其中 k = var[k]ˉ / (var[k]ˉ + sample_var) (var 和 sample_var 分别为预测误差方差和观测误差方差)
bitrate_estimate_kbps_ = (sample_var * bitrate_estimate_kbps_ +
                            pred_bitrate_estimate_var * bitrate_sample_kbps) /
                           (sample_var + pred_bitrate_estimate_var);
bitrate_estimate_kbps_ =
      std::max(bitrate_estimate_kbps_, estimate_floor_.Get().kbps<float>());
// 这其实对应的是一个卡尔曼率滤波的后验方差的更新过程,
// 后验方差: var[k] = (1 - k) * var[k]ˉ
// 其中 k = var[k]ˉ / (var[k]ˉ + sample_var) (var 和 sample_var 分别为预测误差方差和观测误差方差)
bitrate_estimate_var_ = sample_var * pred_bitrate_estimate_var /
                          (sample_var + pred_bitrate_estimate_var);
BWE_TEST_LOGGING_PLOT(1, "acknowledged_bitrate", at_time.ms(),
                        bitrate_estimate_kbps_ * 1000);

```
> constexpr int kInitialRateWindowMs = 500;
> constexpr int kRateWindowMs = 150;

  当设置了 ALR 时间后, 估计的系数 ExpectFastRateChange() 更新, 可以让后面的预计值更信任观测值, 也就是变化更快.


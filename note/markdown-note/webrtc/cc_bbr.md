
## BbrNetworkController

### BandwidthSampler

在每次收到 ack 包的时候 OnPacketAcknowledged(), 计算一个 send rate 和一个 ack rate 然后选择 MIN 返回，
计算过程中，保证计算的 packet send 和 ack 时间为递增.

### WindowedFilter

记录一个窗口时间内的，第一/二/三好的值; 其中第二好值在 > 4/window 时间更新, 第三好值在 > 2 / window 内更新;
保证了三个值在窗口内的有效分布.


根据 RFC [draft-cardwell-iccrg-bbr-congestion-control] 实现，模块对外返回:
NetworkControlUpdate.pacer_config 来控制发送速率
NetworkControlUpdate.target_rate 来控制各个模块的带宽分配

具体来说，首先根据四个状态 STARTUP, DRAIN, PROBE_BW ,PROBE_RTT 来决定 **pacing_gain_** 和 **congestion_window_gain_**,
- 当处于 STARTUP 时，预期需要快速来达到 BBR 中的最佳传输速率；所以两个 gain 都会调的比较大，为 2.885
- 当带宽连续 3 次都无法超过上次带宽的 1.25 倍时，进入 DRAIN 状态; 此时两个 gain 分别为 0.347 和 2.885; 用于消耗排队中的数据
- 当发送中的数据比拥塞窗口小时，表示排队数据已经被发送完成；进入 PROBE_BW 状态, 用于保持 BBR 的高吞吐量，低延时; 按照 RFC **pacing_gain_**
会依次 5/4, 3/4, 1, 1, 1, 1, 1, 1 按照这 8 个状态来循环, **congestion_window_gain_** 为 2，为了预防网络中存在的任何突发状况
- 当 RTT 超过 10s 没有有效更新时，会降低  **pacing_gain_** 到 1，然后把拥塞窗口设置初始值，获取有效的 RTT

当发生丢包时，BBR 将策略控制为较为保守，以适应网络中发生的变化; 具体定义了 CONSERVATION, MEDIUM_GROWTH, GROWTH 几个状态:
- CONSERVATION 当发生丢包时，**congestion_window_gain_** 为已经 acked 数据 + 正在发送中的数据
- 然后 **congestion_window_gain_** 增加下一次 acked 数据, 直到退出丢包状态

## docs
https://github.com/google/bbr
https://blog.acolyer.org/2017/03/31/bbr-congestion-based-congestion-control/
https://datatracker.ietf.org/doc/html/draft-cardwell-iccrg-bbr-congestion-control#section-3.1
https://www.ietf.org/proceedings/97/slides/slides-97-iccrg-bbr-congestion-control-02.pdf
https://queue.acm.org/detail.cfm?id=3022184
https://ee.lbl.gov/papers/congavoid.pdf

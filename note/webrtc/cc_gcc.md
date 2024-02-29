  GoogCcNetworkController 是 google 实现的一套拥塞算法，主要是实现高灵敏度的音视频传输;
模块对外主要输出 NetworkControlUpdate.target_rate 用于各个模块的带宽分配，NetworkControlUpdate.pacer_config 用于平滑发送，
gcc 默认没有拥塞窗口值，主要依靠灵敏的反馈来进行拥塞控制; 其中 pacer_config 主要就是带宽估计值乘以一个系数，默认为 kDefaultPaceMultiplier.

  带宽估计由 SendSideBandwidthEstimation 统一管理，它可能接受来自三个来源的反馈:
- RTCP 的反馈丢包 UpdatePacketsLost(), 然后根据丢包范围，快降慢升, 计算一个带宽
- REMB 反馈带宽 UpdateReceiverEstimate()
- TransportFeedback 反馈，经过 DelayBasedBwe 模块估算后的带宽

这三个带宽值，取最小的一个使用.

其中 DelayBasedBwe 有几个模块组成，当每个 TransportFeedback 达到时
- GoogCcNetworkController 通过 AcknowledgedBitrateEstimator 来获取当前的实际吞吐带宽;
AcknowledgedBitrateEstimator 主要通过 BitrateEstimator 中的贝叶斯估计来实现
- DelayBasedBwe 内部通过 InterArrival 将 packet 分组计算出 send_delta, recv_delta; 然后把数据在 TrendlineEstimator 模块中做
线性拟合，得出一个 BandwidthUsage 的状态
- DelayBasedBwe 内部将 ack 码率和 BandwidthUsage 送入 AimdRateControl 模块，做最后的带宽更新; 其中 ack 码率和带宽上升的
最大值相关, BandwidthUsage 用于决定是否以一定系数进行带宽增减.




   webrtc 码率探测主要是, 根据需要探测的带宽值, 发送一组数据包, 并通过 feedback 来
 获取这组数据包的到达接收端的花费时长, 来算出带宽值.

   首先, ProbeController 模块在初始化时, 或者在 ALR 状态时, GoogCcNetworkController 通过主动调用
ProbeController::RequestProbe() 或者定时调用 ProbeController::Process() 后, 将返回目前需要探测的
ProbeClusterConfig 信息:
```c++
ProbeClusterConfig.target_data_rate
// next_probe_cluster_id_
ProbeClusterConfig.id
...
```

  然后, config 信息将会放到 NetworkControlUpdate.probe_cluster_configs, 通过 RtpTransportControllerSend::PostUpdates()
进行处理. 具体来说, 就是 PacedSender 通过 BitrateProber 来创建对应的 ProbeCluster:
```c++
ProbeCluster.pace_info.probe_cluster_min_bytes
ProbeCluster.pace_info.send_bitrate_bps
ProbeCluster.pace_info.probe_cluster_id
...
```
  PacedSender 在每次发送时, 去向 BitrateProber 获取是否在 probe; 如果在 probe, 则会根据 BitrateProber::TimeUntilNextProbe()
来获取更新 PacedSender 模块下次处理的时间; 这个时间, 其实就是根据需要探测的带宽来计算的发送数据间隔:
```c++
int64_t BitrateProber::GetNextProbeTime(const ProbeCluster& cluster) {
  // Compute the time delta from the cluster start to ensure probe bitrate stays
  // close to the target bitrate. Result is in milliseconds.
  int64_t delta_ms =
      (8000ll * cluster.sent_bytes + cluster.pace_info.send_bitrate_bps / 2) /
      cluster.pace_info.send_bitrate_bps;
  return cluster.time_started_ms + delta_ms;
}
```
每次 probe 时需要发送的数据, 则会根据 BitrateProber::RecommendedMinProbeSize() 来返回, 如果媒体数据不足, 则会补上 padding 数据;
当前一次 probe 的数据在 BitrateProber::ProbeSent() 中判定满足条件后, 当前发送探测包就完成了:
```c++
...
if (cluster->sent_bytes >= (cluster->pace_info.probe_cluster_min_bytes * kProbePercentOverMinRequire / 100) &&
        cluster->sent_probes >= (cluster->pace_info.probe_cluster_min_probes * kProbePercentOverMinRequire / 100)) {
  clusters_.pop();
}
...
```

  最后, 探测包发送后, 数据包到达的时间和数据多少, 通过 feedback 可以计算出来, 由此来计算带宽. 这里 feedback 的数据包, 需要对应找到 probe
的 cluster, 是在 TransportFeedbackAdapter 中完成的; 发送数据时, 会通过 AddPacket() 保存, 接收到 feedback 后再通过调用 ProcessTransportFeedback()
来对应获取, 放置在 PacketFeedback.pacing_info; 具体的计算过程在 ProbeBitrateEstimator::HandleProbeAndEstimateBitrate():
```c++
  ...
  if (send_interval_ms <= 0 || send_interval_ms > kMaxProbeIntervalMs ||
      receive_interval_ms <= 0 || receive_interval_ms > kMaxProbeIntervalMs) {
    return -1;
  }
  ...
  float receive_size = cluster->size_total - cluster->size_first_receive;
  float receive_bps = receive_size / receive_interval_ms * 1000;
  ...
```
这里如果，探测的时长超过阈值或则其他情况，则为探测无效；如果有效时，带宽则是数据量和时间的比值.
  


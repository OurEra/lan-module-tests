### send side bitrate by receiver report
```c++
webrtc::SendSideBandwidthEstimation::UpdateEstimate(webrtc::Timestamp) at send_side_bandwidth_estimation.cc:563
webrtc::SendSideBandwidthEstimation::UpdatePacketsLost(int, int, webrtc::Timestamp) at send_side_bandwidth_estimation.cc:401
webrtc::GoogCcNetworkController::OnTransportLossReport(webrtc::TransportLossReport) at goog_cc_network_control.cc:369
webrtc::RtpTransportControllerSend::OnReceivedRtcpReceiverReportBlocks(std::list<webrtc::RTCPReportBlock, std::allocator<webrtc::RTCPReportBlock> > const&, long) at rtp_transport_controller_send.cc:575
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::RtpTransportControllerSend::OnReceivedRtcpReceiverReport(std::list<webrtc::RTCPReportBlock, std::allocator<webrtc::RTCPReportBlock> > const&, long, long)::$_9>::Run() webrtc::RtpTransportControllerSend::OnReceivedRtcpReceiverReport(std::list<webrtc::RTCPReportBlock, std::allocator<webrtc::RTCPReportBlock> > const&, long, long)::$_9::operator()() const at rtp_transport_controller_send.cc:397
```

### send side bitrate by remb
```c++
webrtc::SendSideBandwidthEstimation::CapBitrateToThresholds(webrtc::Timestamp, webrtc::DataRate, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&) at send_side_bandwidth_estimation.cc:672
webrtc::SendSideBandwidthEstimation::UpdateReceiverEstimate(webrtc::Timestamp, webrtc::DataRate) at send_side_bandwidth_estimation.cc:330
webrtc::GoogCcNetworkController::OnRemoteBitrateReport(webrtc::RemoteBitrateReport) at goog_cc_network_control.cc:232
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::RtpTransportControllerSend::OnReceivedEstimatedBitrate(unsigned int)::$_8>::Run() [inlined] webrtc::RtpTransportControllerSend::OnReceivedEstimatedBitrate(unsigned int)::$_8::operator()() const at rtp_transport_controller_send.cc:387
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::RtpTransportControllerSend::OnReceivedEstimatedBitrate(unsigned int)::$_8>::Run() at to_queued_task.h:32
```

### send side bitrate by feedabck
```c++
webrtc::SendSideBandwidthEstimation::CapBitrateToThresholds(webrtc::Timestamp, webrtc::DataRate, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&) at send_side_bandwidth_estimation.cc:672
webrtc::SendSideBandwidthEstimation::UpdateDelayBasedEstimate(webrtc::Timestamp, webrtc::DataRate) at send_side_bandwidth_estimation.cc:341
webrtc::GoogCcNetworkController::OnTransportPacketsFeedback(webrtc::TransportPacketsFeedback) at goog_cc_network_control.cc:499
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::RtpTransportControllerSend::OnTransportFeedback(webrtc::rtcp::TransportFeedback const&)::$_11>::Run() [inlined] webrtc::RtpTransportControllerSend::OnTransportFeedback(webrtc::rtcp::TransportFeedback const&)::$_11::operator()() const at rtp_transport_controller_send.cc:434
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::RtpTransportControllerSend::OnTransportFeedback(webrtc::rtcp::TransportFeedback const&)::$_11>::Run() at to_queued_task.h:32
webrtc::(anonymous namespace)::TaskQueueLibevent::OnWakeup(int, short, void*) at task_queue_libevent.cc:295
```

> parse rtcp::TransportFeedback
```c++
void RTCPReceiver::HandleTransportFeedback(
    const CommonHeader& rtcp_block,
    PacketInformation* packet_information) {
  std::unique_ptr<rtcp::TransportFeedback> transport_feedback(
      new rtcp::TransportFeedback());
  if (!transport_feedback->Parse(rtcp_block)) {
    ++num_skipped_packets_;
    return;
  }

  packet_information->packet_type_flags |= kRtcpTransportFeedback;
  packet_information->transport_feedback = std::move(transport_feedback);
}
```

> rtcp::TransportFeedback to PacketFeedback
```c++
  packet_feedback_vector.reserve(feedback.GetPacketStatusCount());
  {
    rtc::CritScope cs(&lock_);
    size_t failed_lookups = 0;
    int64_t offset_us = 0;
    int64_t timestamp_ms = 0;
    uint16_t seq_num = feedback.GetBaseSequence();
    for (const auto& packet : feedback.GetReceivedPackets()) {
      // Insert into the vector those unreceived packets which precede this
      // iteration's received packet.
      for (; seq_num != packet.sequence_number(); ++seq_num) {
        PacketFeedback packet_feedback(PacketFeedback::kNotReceived, seq_num);
        // Note: Element not removed from history because it might be reported
        // as received by another feedback.
        if (!send_time_history_.GetFeedback(&packet_feedback, false))
          ++failed_lookups;
        if (packet_feedback.local_net_id == local_net_id_ &&
            packet_feedback.remote_net_id == remote_net_id_) {
          packet_feedback_vector.push_back(packet_feedback);
        }
      }

      // Handle this iteration's received packet.
      offset_us += packet.delta_us();
      timestamp_ms = current_offset_ms_ + (offset_us / 1000);
      PacketFeedback packet_feedback(timestamp_ms, packet.sequence_number());
      if (!send_time_history_.GetFeedback(&packet_feedback, true))
        ++failed_lookups;
      if (packet_feedback.local_net_id == local_net_id_ &&
          packet_feedback.remote_net_id == remote_net_id_) {
        packet_feedback_vector.push_back(packet_feedback);
      }

      ++seq_num;
    }

    if (failed_lookups > 0) {
      RTC_LOG(LS_WARNING) << "Failed to lookup send time for " << failed_lookups
                          << " packet" << (failed_lookups > 1 ? "s" : "")
                          << ". Send time history too small?";
    }
  }
```

> PacketFeedback to TransportPacketsFeedback
```c++
  for (const PacketFeedback& rtp_feedback : feedback_vector) {
    if (rtp_feedback.send_time_ms != PacketFeedback::kNoSendTime) {
      auto feedback = NetworkPacketFeedbackFromRtpPacketFeedback(rtp_feedback);
      msg.packet_feedbacks.push_back(feedback);
    } else if (rtp_feedback.arrival_time_ms == PacketFeedback::kNotReceived) {
      msg.sendless_arrival_times.push_back(Timestamp::PlusInfinity());
    } else {
      msg.sendless_arrival_times.push_back(
          Timestamp::ms(rtp_feedback.arrival_time_ms));
    }
  }
  {
    rtc::CritScope cs(&lock_);
    absl::optional<int64_t> first_unacked_send_time_ms =
        send_time_history_.GetFirstUnackedSendTime();
    if (first_unacked_send_time_ms)
      msg.first_unacked_send_time = Timestamp::ms(*first_unacked_send_time_ms);
  }
  msg.feedback_time = feedback_receive_time;
  msg.prior_in_flight = prior_in_flight;
  msg.data_in_flight = GetOutstandingData();
```


### hack to disable TCC
```c++
--- a/media/engine/webrtc_video_engine.cc
+++ b/media/engine/webrtc_video_engine.cc
@@ -70,8 +70,8 @@ void AddDefaultFeedbackParams(VideoCodec* codec) {
   if (codec->name == kRedCodecName || codec->name == kUlpfecCodecName)
     return;
   codec->AddFeedbackParam(FeedbackParam(kRtcpFbParamRemb, kParamValueEmpty));
-  codec->AddFeedbackParam(
-      FeedbackParam(kRtcpFbParamTransportCc, kParamValueEmpty));
+  //codec->AddFeedbackParam(
+  //    FeedbackParam(kRtcpFbParamTransportCc, kParamValueEmpty));
   // Don't add any more feedback params for FLEXFEC.
   if (codec->name == kFlexfecCodecName || codec->name == kRSfecCodecName)
     return;
@@ -512,8 +512,8 @@ RtpCapabilities WebRtcVideoEngine::GetCapabilities() const {
       webrtc::RtpExtension(webrtc::RtpExtension::kAbsSendTimeUri, id++));
   capabilities.header_extensions.push_back(
       webrtc::RtpExtension(webrtc::RtpExtension::kVideoRotationUri, id++));
-  capabilities.header_extensions.push_back(webrtc::RtpExtension(
-      webrtc::RtpExtension::kTransportSequenceNumberUri, id++));
+  //capabilities.header_extensions.push_back(webrtc::RtpExtension(
+  //    webrtc::RtpExtension::kTransportSequenceNumberUri, id++));
   capabilities.header_extensions.push_back(
       webrtc::RtpExtension(webrtc::RtpExtension::kPlayoutDelayUri, id++));
   capabilities.header_extensions.push_back(
diff --git a/media/engine/webrtc_voice_engine.cc b/media/engine/webrtc_voice_engine.cc
index 60323224fa..d819a4d967 100644
--- a/media/engine/webrtc_voice_engine.cc
+++ b/media/engine/webrtc_voice_engine.cc
@@ -556,8 +556,8 @@ RtpCapabilities WebRtcVoiceEngine::GetCapabilities() const {
   int id = 1;
   capabilities.header_extensions.push_back(
       webrtc::RtpExtension(webrtc::RtpExtension::kAudioLevelUri, id++));
-  capabilities.header_extensions.push_back(webrtc::RtpExtension(
-      webrtc::RtpExtension::kTransportSequenceNumberUri, id++));
+  //capabilities.header_extensions.push_back(webrtc::RtpExtension(
+  //    webrtc::RtpExtension::kTransportSequenceNumberUri, id++));
   return capabilities;
 }
 
@@ -652,8 +652,8 @@ AudioCodecs WebRtcVoiceEngine::CollectCodecs(
     if (opt_codec) {
       AudioCodec& codec = *opt_codec;
       if (spec.info.supports_network_adaption) {
-        codec.AddFeedbackParam(
-            FeedbackParam(kRtcpFbParamTransportCc, kParamValueEmpty));
+        //codec.AddFeedbackParam(
+        //    FeedbackParam(kRtcpFbParamTransportCc, kParamValueEmpty));
       }
 
       if (spec.info.allow_comfort_noise) {
```

###
SendSideBandwidthEstimation

#### ALR
AlrDetector
https://bugs.chromium.org/p/webrtc/issues/detail?id=6332

#### PROBE
ProbeBitrateEstimator
https://bugs.chromium.org/p/webrtc/issues/detail?id=5859

DelayBasedBwe

AcknowledgedBitrateEstimator

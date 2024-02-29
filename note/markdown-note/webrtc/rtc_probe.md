
### estimate bitrate base on feedback
> RTPFB: Transport layer feedback message.
> [RFC4585, Section 6.4](https://tools.ietf.org/html/rfc4585#section-6.4)
> [transport-wide-cc](https://datatracker.ietf.org/doc/html/draft-holmer-rmcat-transport-wide-cc-extensions-01)

> //    Message format
> //
> //     0                   1                   2                   3
> //     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
> //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
> //    |V=2|P|  FMT=15 |    PT=205     |           length              |
> //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
> //  0 |                     SSRC of packet sender                     |
> //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
> //  4 |                      SSRC of media source                     |
> //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
> //  8 |      base sequence number     |      packet status count      |
> //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
> // 12 |                 reference time                | fb pkt. count |
> //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
> // 16 |          packet chunk         |         packet chunk          |
> //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
> //    .                                                               .
> //    .                                                               .
> //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
> //    |         packet chunk          |  recv delta   |  recv delta   |
> //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
> //    .                                                               .
> //    .                                                               .
> //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
> //    |           recv delta          |  recv delta   | zero padding  |
> //    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


```
webrtc::ProbeBitrateEstimator::HandleProbeAndEstimateBitrate(webrtc::PacketFeedback const&) at probe_bitrate_estimator.cc:169
webrtc::GoogCcNetworkController::OnTransportPacketsFeedback(webrtc::TransportPacketsFeedback) at goog_cc_network_control.cc:465
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::RtpTransportControllerSend::OnTransportFeedback(webrtc::rtcp::TransportFeedback const&)::$_11>::Run() [inlined] webrtc::RtpTransportControllerSend::OnTransportFeedback(webrtc::rtcp::TransportFeedback const&)::$_11::operator()() const at rtp_transport_controller_send.cc:434
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::RtpTransportControllerSend::OnTransportFeedback(webrtc::rtcp::TransportFeedback const&)::$_11>::Run() at to_queued_task.h:32
webrtc::(anonymous namespace)::TaskQueueLibevent::OnWakeup(int, short, void*) at task_queue_libevent.cc:295
```

### remote side send feedback
```
webrtc::ModuleRtpRtcpImpl::SendFeedbackPacket(webrtc::rtcp::TransportFeedback const&) at rtp_rtcp_impl.cc:732
webrtc::PacketRouter::SendTransportFeedback(webrtc::rtcp::TransportFeedback*) at packet_router.cc:258
webrtc::RemoteEstimatorProxy::SendPeriodicFeedbacks() at remote_estimator_proxy.cc:186
webrtc::RemoteEstimatorProxy::Process() at remote_estimator_proxy.cc:87
webrtc::ProcessThreadImpl::Process() at process_thread_impl.cc:192
```

### create cluster
```
webrtc::BitrateProber::CreateProbeCluster(int, long, int) at bitrate_prober.cc:102
webrtc::PacedSender::CreateProbeCluster(int, int) at paced_sender.cc:99
webrtc::RtpTransportControllerSend::PostUpdates(webrtc::NetworkControlUpdate) at rtp_transport_controller_send.cc:526
webrtc::RtpTransportControllerSend::UpdateControllerWithTimeInterval() at rtp_transport_controller_send.cc:504
webrtc::RtpTransportControllerSend::MaybeCreateControllers() at rtp_transport_controller_send.cc:463
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::RtpTransportControllerSend::OnNetworkAvailability(bool)::$_3>::Run() [inlined] webrtc::RtpTransportControllerSend::OnNetworkAvailability(bool)::$_3::operator()() const at rtp_transport_controller_send.cc:284
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::RtpTransportControllerSend::OnNetworkAvailability(bool)::$_3>::Run() at to_queued_task.h:32
webrtc::(anonymous namespace)::TaskQueueLibevent::OnWakeup(int, short, void*) at task_queue_libevent.cc:295
```

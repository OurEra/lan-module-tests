
### IceTransportInternal
```c++
JsepTransportController::CreateIceTransport(const std::string transport_name,
                                            bool rtcp) {
    return absl::make_unique<cricket::P2PTransportChannel>(
        transport_name, component, port_allocator_, async_resolver_factory_,
        config_.event_log);
```


### DtlsTransport
```c++
JsepTransportController::CreateDtlsTransport(
    dtls = absl::make_unique<cricket::DtlsTransport>(
        std::move(ice), config_.crypto_options, config_.event_log);
```


### DtlsSrtpTransport
```c++
JsepTransportController::CreateDtlsSrtpTransport(
  auto dtls_srtp_transport = absl::make_unique<webrtc::DtlsSrtpTransport>(
      rtcp_dtls_transport == nullptr);

```

### add connection
```c++
cricket::P2PTransportChannel::AddConnection(cricket::Connection*) at p2p_transport_channel.cc:192
cricket::P2PTransportChannel::CreateConnection(cricket::PortInterface*, cricket::Candidate const&, cricket::PortInterface*) at p2p_transport_channel.cc:1266
cricket::P2PTransportChannel::CreateConnections(cricket::Candidate const&, cricket::PortInterface*) at p2p_transport_channel.cc:1224
cricket::P2PTransportChannel::FinishAddingRemoteCandidate(cricket::Candidate const&) at p2p_transport_channel.cc:1171
cricket::P2PTransportChannel::AddRemoteCandidate(cricket::Candidate const&) at p2p_transport_channel.cc:1102
cricket::JsepTransport::AddRemoteCandidates(std::vector<cricket::Candidate, std::allocator<cricket::Candidate> > const&) at jsep_transport.cc:321
webrtc::JsepTransportController::AddRemoteCandidates(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::vector<cricket::Candidate, std::allocator<cricket::Candidate> > const&) at jsep_transport_controller.cc:322
rtc::FunctorMessageHandler<webrtc::RTCError, webrtc::JsepTransportController::AddRemoteCandidates(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::vector<cricket::Candidate, std::allocator<cricket::Candidate> > const&)::$_9>::OnMessage(rtc::Message*)  webrtc::JsepTransportController::AddRemoteCandidates(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::vector<cricket::Candidate, std::allocator<cricket::Candidate> > const&)::$_9::operator()() const at jsep_transport_controller.cc:307
rtc::FunctorMessageHandler<webrtc::RTCError, webrtc::JsepTransportController::AddRemoteCandidates(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::vector<cricket::Candidate, std::allocator<cricket::Candidate> > const&)::$_9>::OnMessage(rtc::Message*) at message_handler.h:42
rtc::MessageQueue::Dispatch(rtc::Message*) at message_queue.cc:513
rtc::Thread::ReceiveSendsFromThread(rtc::Thread const*) at thread.cc:458
rtc::MessageQueue::Get(rtc::Message*, int, bool) at message_queue.cc:261
```

### create socket
```c++
rtc::AsyncUDPSocket::AsyncUDPSocket(rtc::AsyncSocket*) at async_udp_socket.cc:46
rtc::BasicPacketSocketFactory::CreateUdpSocket(rtc::SocketAddress const&, unsigned short, unsigned short) at basic_packet_socket_factory.cc:57
cricket::AllocationSequence::Init() at basic_port_allocator.cc:1211
cricket::BasicPortAllocatorSession::DoAllocate(bool) at basic_port_allocator.cc:811
cricket::BasicPortAllocatorSession::OnAllocate() at basic_port_allocator.cc:666
rtc::MessageQueue::Dispatch(rtc::Message*) at message_queue.cc:513
rtc::Thread::ProcessMessages(int) at thread.cc:537
```

### send ping
```c++
rtc::AsyncUDPSocket::SendTo(void const*, unsigned long, rtc::SocketAddress const&, rtc::PacketOptions const&) at async_udp_socket.cc:84
cricket::UDPPort::SendTo(void const*, unsigned long, rtc::SocketAddress const&, rtc::PacketOptions const&, bool) at stun_port.cc:295
cricket::Connection::OnSendStunPacket(void const*, unsigned long, cricket::StunRequest*) at port.cc:1246
cricket::StunRequest::OnMessage(rtc::Message*) at stun_request.cc:251
rtc::Thread::Send(rtc::Location const&, rtc::MessageHandler*, unsigned int, rtc::MessageData*) at thread.cc:390
cricket::StunRequestManager::SendDelayed(cricket::StunRequest*, int) at stun_request.cc:73
cricket::Connection::Ping(long) at port.cc:1539
cricket::P2PTransportChannel::CheckAndPing() at p2p_transport_channel.cc:2022
rtc::AsyncInvoker::OnMessage(rtc::Message*) at async_invoker.cc:45
```

### construct stun request
```c++
cricket::StunMessage::Write(rtc::ByteBufferWriter*) const at stun.cc:429
cricket::StunMessage::AddMessageIntegrity(char const*, unsigned long) at stun.cc:274
cricket::ConnectionRequest::Prepare(cricket::StunMessage*) at port.cc:1070
cricket::StunRequest::Construct() at stun_request.cc:210
cricket::StunRequestManager::SendDelayed(cricket::StunRequest*, int) at stun_request.cc:68
cricket::Connection::Ping(long) at port.cc:1541
cricket::P2PTransportChannel::CheckAndPing() at p2p_transport_channel.cc:2019
rtc::AsyncInvoker::OnMessage(rtc::Message*) at async_invoker.cc:45
```

### send audio packet to rtp sender
```c++
webrtc::RTPSender::SendToNetwork(std::unique_ptr<webrtc::RtpPacketToSend, std::default_delete<webrtc::RtpPacketToSend> >, webrtc::StorageType, webrtc::RtpPacketSender::Priority) at rtp_sender.cc:685
webrtc::RTPSenderAudio::LogAndSendToNetwork(std::unique_ptr<webrtc::RtpPacketToSend, std::default_delete<webrtc::RtpPacketToSend> >, webrtc::StorageType, webrtc::RtpPacketSender::Priority) at rtp_sender_audio.cc:363
webrtc::RTPSenderAudio::SendAudio(webrtc::AudioFrameType, signed char, unsigned int, unsigned char const*, unsigned long) at rtp_sender_audio.cc:257
webrtc::voe::ChannelSend::SendData(webrtc::AudioFrameType, unsigned char, unsigned int, unsigned char const*, unsigned long, webrtc::RTPFragmentationHeader const*)  webrtc::voe::ChannelSend::SendRtpAudio(webrtc::AudioFrameType, unsigned char, unsigned int, rtc::ArrayView<unsigned char const, -4711l>, webrtc::RTPFragmentationHeader const*) at channel_send.cc:564
webrtc::voe::ChannelSend::SendData(webrtc::AudioFrameType, unsigned char, unsigned int, unsigned char const*, unsigned long, webrtc::RTPFragmentationHeader const*) at channel_send.cc:498
webrtc::AudioCodingModuleImpl::Add10MsData(webrtc::AudioFrame const&)  webrtc::AudioCodingModuleImpl::Encode(webrtc::AudioCodingModuleImpl::InputData const&) at audio_coding_module.cc:409
webrtc::AudioCodingModuleImpl::Add10MsData(webrtc::AudioFrame const&) at audio_coding_module.cc:456
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::voe::ChannelSend::ProcessAndEncodeAudio(std::unique_ptr<webrtc::AudioFrame, std::default_delete<webrtc::AudioFrame> >)::ProcessAndEncodeAudio>::Run()  webrtc::voe::ChannelSend::ProcessAndEncodeAudioOnTaskQueue(webrtc::AudioFrame*) at channel_send.cc:1146
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::voe::ChannelSend::ProcessAndEncodeAudio(std::unique_ptr<webrtc::AudioFrame, std::default_delete<webrtc::AudioFrame> >)::ProcessAndEncodeAudio>::Run()  webrtc::voe::ChannelSend::ProcessAndEncodeAudio(std::unique_ptr<webrtc::AudioFrame, std::default_delete<webrtc::AudioFrame> >)::ProcessAndEncodeAudio::operator()() at channel_send.cc:1102
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::voe::ChannelSend::ProcessAndEncodeAudio(std::unique_ptr<webrtc::AudioFrame, std::default_delete<webrtc::AudioFrame> >)::ProcessAndEncodeAudio>::Run() at to_queued_task.h:32
webrtc::TaskQueueLibevent::OnWakeup(int, short, void*) at task_queue_libevent.cc:295
event_base_loop  event_process_active at event.c:381
````

### send packet to channel
```c++
cricket::BaseChannel::SendPacket(bool, rtc::CopyOnWriteBuffer*, rtc::PacketOptions const&) at channel.cc:413
cricket::MediaChannel::DoSendPacket(rtc::CopyOnWriteBuffer*, bool, rtc::PacketOptions const&) at media_channel.h:0
cricket::WebRtcVoiceMediaChannel::SendRtp(unsigned char const*, unsigned long, webrtc::PacketOptions const&) at webrtc_voice_engine.h:233
webrtc::RTPSender::SendPacketToNetwork(webrtc::RtpPacketToSend const&, webrtc::PacketOptions const&, webrtc::PacedPacketInfo const&) at rtp_sender.cc:491
webrtc::RTPSender::PrepareAndSendPacket(std::unique_ptr<webrtc::RtpPacketToSend, std::default_delete<webrtc::RtpPacketToSend> >, bool, bool, webrtc::PacedPacketInfo const&) at rtp_sender.cc:615
webrtc::RTPSender::TimeToSendPacket(unsigned int, unsigned short, long, bool, webrtc::PacedPacketInfo const&) at rtp_sender.cc:543
webrtc::PacketRouter::TimeToSendPacket(unsigned int, unsigned short, long, bool, webrtc::PacedPacketInfo const&) at packet_router.cc:119
webrtc::PacedSender::Process() at paced_sender.cc:375
webrtc::ProcessThreadImpl::Process() at process_thread_impl.cc:192
rtc::PlatformThread::Run() at platform_thread.cc:190
```

### send packet to transport
```c++
rtc::AsyncUDPSocket::SendTo(void const*, unsigned long, rtc::SocketAddress const&, rtc::PacketOptions const&) at async_udp_socket.cc:89
cricket::UDPPort::SendTo(void const*, unsigned long, rtc::SocketAddress const&, rtc::PacketOptions const&, bool) at stun_port.cc:295
cricket::ProxyConnection::Send(void const*, unsigned long, rtc::PacketOptions const&) at port.cc:1981
cricket::P2PTransportChannel::SendPacket(char const*, unsigned long, rtc::PacketOptions const&, int) at p2p_transport_channel.cc:1405
webrtc::RtpTransport::SendPacket(bool, rtc::CopyOnWriteBuffer*, rtc::PacketOptions const&, int) at rtp_transport.cc:132
webrtc::SrtpTransport::SendRtpPacket(rtc::CopyOnWriteBuffer*, rtc::PacketOptions const&, int) at srtp_transport.cc:173
cricket::BaseChannel::SendPacket(bool, rtc::CopyOnWriteBuffer*, rtc::PacketOptions const&) at channel.cc:0
cricket::BaseChannel::OnMessage(rtc::Message*) at channel.cc:761
rtc::MessageQueue::Dispatch(rtc::Message*) at message_queue.cc:513
```

# stats
### collect stats from pc api
```c++
webrtc::RTCStatsCollector::ProduceMediaStreamTrackStats_s(long, webrtc::RTCStatsReport*) const  webrtc::ProduceMediaStreamTrackStatsFromVoiceSenderInfo(long, webrtc::AudioTrackInterface const&, cricket::VoiceSenderInfo const&, int) at rtc_stats_collector.cc:474
webrtc::RTCStatsCollector::ProduceMediaStreamTrackStats_s(long, webrtc::RTCStatsReport*) const  webrtc::ProduceSenderMediaTrackStats(long, webrtc::TrackMediaInfoMap const&, std::vector<rtc::scoped_refptr<webrtc::RtpSenderInternal>, std::allocator<rtc::scoped_refptr<webrtc::RtpSenderInternal> > >, webrtc::RTCStatsReport*) at rtc_stats_collector.cc:637
webrtc::RTCStatsCollector::ProduceMediaStreamTrackStats_s(long, webrtc::RTCStatsReport*) const at rtc_stats_collector.cc:1264
webrtc::RTCStatsCollector::ProducePartialResultsOnSignalingThreadImpl(long, webrtc::RTCStatsReport*) at rtc_stats_collector.cc:938
webrtc::RTCStatsCollector::ProducePartialResultsOnSignalingThread(long) at rtc_stats_collector.cc:921
webrtc::RTCStatsCollector::GetStatsReportInternal(webrtc::RTCStatsCollector::RequestInfo) at rtc_stats_collector.cc:900
webrtc::RTCStatsCollector::GetStatsReport(rtc::scoped_refptr<webrtc::RTCStatsCollectorCallback>) at rtc_stats_collector.cc:833
webrtc::PeerConnection::GetStats(webrtc::RTCStatsCollectorCallback*) at peer_connection.cc:1853
```
> stats name and value organized in RTCStatsMember, like:
```c++
RTCMediaStreamTrackStats
  RTCStatsMember<double> audio_level;
  RTCStatsMember<double> total_audio_energy;
```

### voice engine get receiver stats
```c++
webrtc::internal::AudioReceiveStream::GetStats() const at audio_receive_stream.cc:238
cricket::WebRtcVoiceMediaChannel::GetStats(cricket::VoiceMediaInfo*) at webrtc_voice_engine.cc:2268
rtc::FunctorMessageHandler<void, webrtc::StatsCollector::ExtractMediaInfo()::$_2>::OnMessage(rtc::Message*)  webrtc::StatsCollector::ExtractMediaInfo()::$_2::operator()() const at stats_collector.cc:1043
```

### video engine get sender stats
```c++
webrtc::SendStatisticsProxy::GetStats() at send_statistics_proxy.cc:733
cricket::WebRtcVideoChannel::WebRtcVideoSendStream::FillBitrateInfo(cricket::BandwidthEstimationInfo*) at webrtc_video_engine.cc:2459
cricket::WebRtcVideoChannel::FillBitrateInfo(cricket::BandwidthEstimationInfo*) at webrtc_video_engine.cc:1432
rtc::MessageQueue::Dispatch(rtc::Message*) at message_queue.cc:513
```

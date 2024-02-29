# VIDEO

### receive and insert frame
```c++
webrtc::video_coding::FrameBuffer::InsertFrame(unique_ptr<webrtc::video_coding::EncodedFrame, default_delete<webrtc::video_coding::EncodedFrame> >) at frame_buffer2.cc:458
webrtc::internal::VideoReceiveStream::OnCompleteFrame(unique_ptr<webrtc::video_coding::EncodedFrame, default_delete<webrtc::video_coding::EncodedFrame> >) at video_receive_stream.cc:548
webrtc::RtpVideoStreamReceiver::OnCompleteFrame(unique_ptr<webrtc::video_coding::EncodedFrame, default_delete<webrtc::video_coding::EncodedFrame> >) at rtp_video_stream_receiver.cc:441
webrtc::video_coding::RtpFrameReferenceFinder::ManageFrame(unique_ptr<webrtc::video_coding::RtpFrameObject, default_delete<webrtc::video_coding::RtpFrameObject> >) at rtp_frame_reference_finder.cc:54
webrtc::RtpVideoStreamReceiver::OnAssembledFrame(unique_ptr<webrtc::video_coding::RtpFrameObject, default_delete<webrtc::video_coding::RtpFrameObject> >) at rtp_video_stream_receiver.cc:426
webrtc::video_coding::PacketBuffer::InsertPacket(webrtc::VCMPacket*) at packet_buffer.cc:135
webrtc::RtpVideoStreamReceiver::OnReceivedPayloadData(unsigned char const*, unsigned long, webrtc::RTPHeader const&, webrtc::RTPVideoHeader const&, webrtc::VideoFrameType, absl::optional<webrtc::RtpGenericFrameDescriptor> const&, bool) at rtp_video_stream_receiver.cc:293
webrtc::RtpVideoStreamReceiver::ReceivePacket(webrtc::RtpPacketReceived const&) at rtp_video_stream_receiver.cc:609
webrtc::RtpVideoStreamReceiver::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_video_stream_receiver.cc:360
webrtc::RtpDemuxer::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_demuxer.cc:177
webrtc::RtpStreamReceiverController::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_stream_receiver_controller.cc:54
webrtc::internal::Call::DeliverRtp(webrtc::MediaType, rtc::CopyOnWriteBuffer, long) at call.cc:1437
webrtc::internal::Call::DeliverPacket(webrtc::MediaType, rtc::CopyOnWriteBuffer, long) at call.cc:1461
cricket::WebRtcVideoChannel::OnPacketReceived(rtc::CopyOnWriteBuffer, long) at webrtc_video_engine.cc:1405
cricket::BaseChannel::ProcessPacket(bool, rtc::CopyOnWriteBuffer const&, long) at channel.cc:544
rtc::AsyncInvoker::OnMessage(rtc::Message*) at async_invoker.cc:45
rtc::MessageQueue::Dispatch(rtc::Message*) at message_queue.cc:513
```
> receive packet and parse h264
```c++
bool RtpDepacketizerH264::Parse() {
  ...
  uint8_t nal_type = payload_data[0] & kTypeMask;
  parsed_payload->video_header()
      .video_type_header.emplace<RTPVideoHeaderH264>();
  if (nal_type == H264::NaluType::kFuA) {
    // Fragmented NAL units (FU-A).
    if (!ParseFuaNalu(parsed_payload, payload_data))
      return false;
  } else {
    // We handle STAP-A and single NALU's the same way here. The jitter buffer
    // will depacketize the STAP-A into NAL units later.
    // TODO(sprang): Parse STAP-A offsets here and store in fragmentation vec.
    if (!ProcessStapAOrSingleNalu(parsed_payload, payload_data))
      return false;
  }
  ...
}
```


### decode frame
```c++
webrtc::jni::VideoDecoderWrapper::Decode(webrtc::EncodedImage const&, bool, webrtc::CodecSpecificInfo const*, long) at video_decoder_wrapper.cc:119
webrtc::VideoDecoderSoftwareFallbackWrapper::Decode(webrtc::EncodedImage const&, bool, long) at video_decoder_software_fallback_wrapper.cc:156
webrtc::VCMGenericDecoder::Decode(webrtc::VCMEncodedFrame const&, long) at generic_decoder.cc:232
webrtc::vcm::VideoReceiver::Decode(webrtc::VCMEncodedFrame const&) at video_receiver.cc:304
webrtk::internal::VideoReceiveStream::HandleEncodedFrame(unique_ptr<webrtc::video_coding::EncodedFrame, default_delete<webrtc::video_coding::EncodedFrame> >) at video_receive_stream.cc:676
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::internal::VideoReceiveStream::StartNextDecode()::DecodeTask>::Run()  webrtc::internal::VideoReceiveStream::StartNextDecode()::DecodeTask::operator()() at video_receive_stream.cc:613
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::internal::VideoReceiveStream::StartNextDecode()::DecodeTask>::Run() at to_queued_task.h:32
webrtc::TaskQueueLibevent::RunTask(int, short, void*) at task_queue_libevent.cc:308
```

### video receive and trigger nack
```c++
webrtc::ModuleRtpRtcpImpl::SendNack(vector<unsigned short, allocator<unsigned short> > const&) at rtp_rtcp_impl.cc:692
webrtc::NackModule::OnReceivedPacket(unsigned short, bool, bool) at nack_module.cc:142
webrtc::RtpVideoStreamReceiver::OnReceivedPayloadData(unsigned char const*, unsigned long, webrtc::RTPHeader const&, webrtc::RTPVideoHeader const&, webrtc::VideoFrameType, absl::optional<webrtc::RtpGenericFrameDescriptor> const&, bool) at rtp_video_stream_receiver.cc:245
webrtc::RtpVideoStreamReceiver::ReceivePacket(webrtc::RtpPacketReceived const&) at rtp_video_stream_receiver.cc:609
webrtc::RtpVideoStreamReceiver::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_video_stream_receiver.cc:360
webrtc::RtpDemuxer::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_demuxer.cc:177
webrtc::RtpStreamReceiverController::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_stream_receiver_controller.cc:54
webrtc::internal::Call::DeliverRtp(webrtc::MediaType, rtc::CopyOnWriteBuffer, long) at call.cc:1437
webrtc::internal::Call::DeliverPacket(webrtc::MediaType, rtc::CopyOnWriteBuffer, long) at call.cc:1461
cricket::WebRtcVideoChannel::OnPacketReceived(rtc::CopyOnWriteBuffer, long) at webrtc_video_engine.cc:1405
cricket::BaseChannel::ProcessPacket(bool, rtc::CopyOnWriteBuffer const&, long) at channel.cc:544
```

### sender side receive nack and do resend
```c++
webrtc::RTPSender::ReSendPacket(unsigned short) at rtp_sender.cc:459
webrtc::RTPSender::OnReceivedNack(vector<unsigned short, allocator<unsigned short> > const&, long) at rtp_sender.cc:515
webrtc::ModuleRtpRtcpImpl::OnReceivedNack(vector<unsigned short, allocator<unsigned short> > const&) at rtp_rtcp_impl.cc:798
webrtc::RTCPReceiver::TriggerCallbacksFromRtcpPacket(webrtc::RTCPReceiver::PacketInformation const&) at rtcp_receiver.cc:1002
webrtc::RTCPReceiver::IncomingPacket(unsigned char const*, unsigned long) at rtcp_receiver.cc:174
webrtc::RtpVideoSender::DeliverRtcp(unsigned char const*, unsigned long) at rtp_video_sender.cc:565
webrtc::internal::Call::DeliverRtcp(webrtc::MediaType, unsigned char const*, unsigned long) at call.cc:1352
cricket::WebRtcVideoChannel::OnRtcpReceived(rtc::CopyOnWriteBuffer, long) at webrtc_video_engine.cc:1519
cricket::BaseChannel::ProcessPacket(bool, rtc::CopyOnWriteBuffer const&, long) at channel.cc:542
```

> nack can send over rtx
```c++
int32_t RTPSender::ReSendPacket(uint16_t packet_id) {

  ...
  const bool rtx = (RtxStatus() & kRtxRetransmitted) > 0;
  if (!PrepareAndSendPacket(std::move(packet), rtx, true, PacedPacketInfo()))
    return -1;

  return packet_size;
}
```

receive time
frame timestamp
=> render time

# AUDIO

### set decoder codecs
```c++
webrtc::DecoderDatabase::SetCodecs(map<int, webrtc::SdpAudioFormat, less<int>, allocator<pair<int const, webrtc::SdpAudioFormat> > > const&) at decoder_database.cc:150
webrtc::NetEqImpl::SetCodecs(map<int, webrtc::SdpAudioFormat, less<int>, allocator<pair<int const, webrtc::SdpAudioFormat> > > const&) at neteq_impl.cc:238
webrtc::AudioCodingModuleImpl::SetReceiveCodecs(map<int, webrtc::SdpAudioFormat, less<int>, allocator<pair<int const, webrtc::SdpAudioFormat> > > const&) at audio_coding_module.cc:679
webrtc::voe::ChannelReceive::SetReceiveCodecs(map<int, webrtc::SdpAudioFormat, less<int>, allocator<pair<int const, webrtc::SdpAudioFormat> > > const&) at channel_receive.cc:576
webrtc::internal::AudioReceiveStream::ConfigureStream(webrtc::internal::AudioReceiveStream*, webrtc::AudioReceiveStream::Config const&, bool) at audio_receive_stream.cc:387
webrtc::internal::AudioReceiveStream::AudioReceiveStream(webrtc::Clock*, webrtc::RtpStreamReceiverControllerInterface*, webrtc::PacketRouter*, webrtc::AudioReceiveStream::Config const&, rtc::scoped_refptr<webrtc::AudioState> const&, webrtc::RtcEventLog*, unique_ptr<webrtc::voe::ChannelReceiveInterface, default_delete<webrtc::voe::ChannelReceiveInterface> >) at audio_receive_stream.cc:135
webrtc::internal::AudioReceiveStream::AudioReceiveStream(webrtc::Clock*, webrtc::RtpStreamReceiverControllerInterface*, webrtc::PacketRouter*, webrtc::ProcessThread*, webrtc::AudioReceiveStream::Config const&, rtc::scoped_refptr<webrtc::AudioState> const&, webrtc::RtcEventLog*) at audio_receive_stream.cc:96
webrtc::internal::Call::CreateAudioReceiveStream(webrtc::AudioReceiveStream::Config const&) at call.cc:784
cricket::WebRtcVoiceMediaChannel::WebRtcAudioReceiveStream::RecreateAudioReceiveStream() at webrtc_voice_engine.cc:1237
cricket::WebRtcVoiceMediaChannel::AddRecvStream(cricket::StreamParams const&) at webrtc_voice_engine.cc:1916
cricket::WebRtcVoiceMediaChannel::OnPacketReceived(rtc::CopyOnWriteBuffer, long) at webrtc_voice_engine.cc:2116
cricket::BaseChannel::ProcessPacket(bool, rtc::CopyOnWriteBuffer const&, long) at channel.cc:544
```

### receive audio packet
```c++
webrtc::acm2::AcmReceiver::InsertPacket(webrtc::RTPHeader const&, rtc::ArrayView<unsigned char const, -4711l>) at acm_receiver.cc:119
webrtc::AudioCodingModuleImpl::IncomingPacket(unsigned char const*, unsigned long, webrtc::RTPHeader const&) at audio_coding_module.cc:693
webrtc::voe::ChannelReceive::OnReceivedPayloadData(unsigned char const*, unsigned long, webrtc::RTPHeader const&) at channel_receive.cc:277
webrtc::voe::ChannelReceive::OnRtpPacket(webrtc::RtpPacketReceived const&)  webrtc::voe::ChannelReceive::ReceivePacket(unsigned char const*, unsigned long, webrtc::RTPHeader const&) at channel_receive.cc:0
webrtc::voe::ChannelReceive::OnRtpPacket(webrtc::RtpPacketReceived const&) at channel_receive.cc:614
webrtc::RtpDemuxer::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_demuxer.cc:177
webrtc::RtpStreamReceiverController::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_stream_receiver_controller.cc:54
webrtc::internal::Call::DeliverRtp(webrtc::MediaType, rtc::CopyOnWriteBuffer, long) at call.cc:1423
webrtc::internal::Call::DeliverPacket(webrtc::MediaType, rtc::CopyOnWriteBuffer, long) at call.cc:1461
cricket::WebRtcVoiceMediaChannel::OnPacketReceived(rtc::CopyOnWriteBuffer, long) at webrtc_voice_engine.cc:2080
cricket::BaseChannel::ProcessPacket(bool, rtc::CopyOnWriteBuffer const&, long) at channel.cc:544
```


### decode audio frame
```c++
webrtc::AudioDecoderOpusImpl::DecodeInternal(unsigned char const*, unsigned long, int, short*, webrtc::AudioDecoder::SpeechType*) at audio_decoder_opus.cc:63
webrtc::AudioDecoder::Decode(unsigned char const*, unsigned long, int, unsigned long, short*, webrtc::AudioDecoder::SpeechType*) at audio_decoder.cc:95
webrtc::OpusFrame::Decode(rtc::ArrayView<short, -4711l>) const at audio_coder_opus_common.h:65
webrtc::NetEqImpl::DecodeLoop(list<webrtc::Packet, allocator<webrtc::Packet> >*, webrtc::Operations const&, webrtc::AudioDecoder*, int*, webrtc::AudioDecoder::SpeechType*) at neteq_impl.cc:1406
webrtc::NetEqImpl::Decode(std::list<webrtc::Packet, std::allocator<webrtc::Packet> >*, webrtc::Operations*, int*, webrtc::AudioDecoder::SpeechType*) at neteq_impl.cc:1321
webrtc::NetEqImpl::GetAudioInternal(webrtc::AudioFrame*, bool*, absl::optional<webrtc::Operations>) at neteq_impl.cc:797
webrtc::NetEqImpl::GetAudio(webrtc::AudioFrame*, bool*, absl::optional<webrtc::Operations>) at neteq_impl.cc:216
webrtc::acm2::AcmReceiver::GetAudio(int, webrtc::AudioFrame*, bool*) at acm_receiver.cc:134
webrtc::AudioCodingModuleImpl::PlayoutData10Ms(int, webrtc::AudioFrame*, bool*) at audio_coding_module.cc:730
webrtc::voe::ChannelReceive::GetAudioFrameWithInfo(int, webrtc::AudioFrame*) at channel_receive.cc:326
webrtc::AudioMixerImpl::GetAudioFromSources() at audio_mixer_impl.cc:186
webrtc::AudioMixerImpl::Mix(unsigned long, webrtc::AudioFrame*) at audio_mixer_impl.cc:130
webrtc::AudioTransportImpl::NeedMorePlayData(unsigned long, unsigned long, unsigned long, unsigned int, void*, unsigned long&, long*, long*) at audio_transport_impl.cc:200
webrtc::AudioDeviceBuffer::RequestPlayoutData(unsigned long) at audio_device_buffer.cc:302
webrtc::jni::AudioTrackJni::GetPlayoutData(_JNIEnv*, webrtc::JavaParamRef<_jobject*> const&, unsigned long) at audio_track_jni.cc:214
```
> audio packet compare
```c++
bool operator<(const Packet& rhs) const {
  if (this->timestamp == rhs.timestamp) {
    if (this->sequence_number == rhs.sequence_number) {
      // Timestamp and sequence numbers are identical - deem the left hand
      // side to be "smaller" (i.e., "earlier") if it has higher priority.
      return this->priority < rhs.priority;
    }
    return (static_cast<uint16_t>(rhs.sequence_number -
                                  this->sequence_number) < 0xFFFF / 2);
  }
  return (static_cast<uint32_t>(rhs.timestamp - this->timestamp) <
          0xFFFFFFFF / 2);
}
```

### there no audio nack for client

# TRANSPORT

### receive remb to gcc module
```c++
`webrtc::GoogCcNetworkController::OnRemoteBitrateReport(webrtc::RemoteBitrateReport) at goog_cc_network_control.cc:232
`webrtc::RtpTransportControllerSend::OnReceivedEstimatedBitrate(unsigned int) at rtp_transport_controller_send.cc:384
`non-virtual thunk to webrtc::RtpTransportControllerSend::OnReceivedEstimatedBitrate(unsigned int) at rtp_transport_controller_send.cc:0
`webrtc::RTCPReceiver::TriggerCallbacksFromRtcpPacket(webrtc::RTCPReceiver::PacketInformation const&) at rtcp_receiver.cc:1042
`webrtc::RTCPReceiver::IncomingPacket(unsigned char const*, unsigned long) at rtcp_receiver.cc:174
`webrtc::RtpVideoSender::DeliverRtcp(unsigned char const*, unsigned long) at rtp_video_sender.cc:565
`webrtc::internal::Call::DeliverRtcp(webrtc::MediaType, unsigned char const*, unsigned long) at call.cc:1352
`cricket::WebRtcVideoChannel::OnRtcpReceived(rtc::CopyOnWriteBuffer, long) at webrtc_video_engine.cc:1519
`cricket::BaseChannel::ProcessPacket(bool, rtc::CopyOnWriteBuffer const&, long) at channel.cc:542
```

### send remb
```c++
`webrtc::RTCPSender::SetRemb(long, vector<unsigned int, allocator<unsigned int> >) at rtcp_sender.cc:239
`webrtc::ModuleRtpRtcpImpl::SetRemb(long, vector<unsigned int, allocator<unsigned int> >) at rtp_rtcp_impl.cc:601
`webrtc::PacketRouter::SendRemb(long, vector<unsigned int, allocator<unsigned int> > const&) at packet_router.cc:249
`webrtc::PacketRouter::OnReceiveBitrateChanged(vector<unsigned int, allocator<unsigned int> > const&, unsigned int) at packet_router.cc:221
`webrtc::RemoteBitrateEstimatorAbsSendTime::IncomingPacketInfo(long, unsigned int, unsigned long, unsigned int) at remote_bitrate_estimator_abs_send_time.cc:360
`webrtc::RemoteBitrateEstimatorAbsSendTime::IncomingPacket(long, unsigned long, webrtc::RTPHeader const&) at remote_bitrate_estimator_abs_send_time.cc:236
`webrtc::ReceiveSideCongestionController::WrappingBitrateEstimator::IncomingPacket(long, unsigned long, webrtc::RTPHeader const&) at receive_side_congestion_controller.cc:43
`webrtc::internal::Call::NotifyBweOfReceivedPacket(webrtc::RtpPacketReceived const&, webrtc::MediaType) at call.cc:1513
`webrtc::internal::Call::DeliverRtp(webrtc::MediaType, rtc::CopyOnWriteBuffer, long) at call.cc:1417
`webrtc::internal::Call::DeliverPacket(webrtc::MediaType, rtc::CopyOnWriteBuffer, long) at call.cc:1461
`cricket::WebRtcVideoChannel::OnPacketReceived(rtc::CopyOnWriteBuffer, long) at webrtc_video_engine.cc:1405
`cricket::BaseChannel::ProcessPacket(bool, rtc::CopyOnWriteBuffer const&, long) at channel.cc:544
```

### send padding for probe
```c++
`webrtc::RTPSender::SendPadData(unsigned long, webrtc::PacedPacketInfo const&) at rtp_sender.cc:410
`webrtc::RTPSender::TimeToSendPadding(unsigned long, webrtc::PacedPacketInfo const&) at rtp_sender.cc:659
`webrtc::PacketRouter::TimeToSendPadding(unsigned long, webrtc::PacedPacketInfo const&) at packet_router.cc:141
`webrtc::PacedSender::Process() at paced_sender.cc:401
```

### send feedback
```c++
`webrtc::RTCPSender::SendFeedbackPacket(webrtc::rtcp::TransportFeedback const&) at rtcp_sender.cc:985
`webrtc::PacketRouter::SendTransportFeedback(webrtc::rtcp::TransportFeedback*) at packet_router.cc:260
`webrtc::RemoteEstimatorProxy::SendPeriodicFeedbacks() at remote_estimator_proxy.cc:186
`webrtc::RemoteEstimatorProxy::Process() at remote_estimator_proxy.cc:87
```

> receiver side use feedback not remb
```
void ReceiveSideCongestionController::OnReceivedPacket(
    int64_t arrival_time_ms,
    size_t payload_size,
    const RTPHeader& header) {
  // Send-side BWE.
  if (header.extension.hasTransportSequenceNumber) {
    remote_estimator_proxy_.IncomingPacket(arrival_time_ms, payload_size,
                                           header);
  } else {
    // Receive-side BWE.
    remote_bitrate_estimator_.IncomingPacket(arrival_time_ms, payload_size,
                                             header);
  }
}
```

### receive feedback
```c++
webrtc::GoogCcNetworkController::OnTransportPacketsFeedback(webrtc::TransportPacketsFeedback) at goog_cc_network_control.cc:382
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::RtpTransportControllerSend::OnTransportFeedback(webrtc::rtcp::TransportFeedback const&)::$_11>::Run()  webrtc::RtpTransportControllerSend::OnTransportFeedback(webrtc::rtcp::TransportFeedback const&)::$_11::operator()() const at rtp_transport_controller_send.cc:434
webrtc::RtpTransportControllerSend::OnTransportFeedback(webrtc::rtcp::TransportFeedback const&) at rtp_transport_controller_send.cc:433
webrtc::RTCPReceiver::TriggerCallbacksFromRtcpPacket(webrtc::RTCPReceiver::PacketInformation const&) at rtcp_receiver.cc:1063
webrtc::RTCPReceiver::IncomingPacket(unsigned char const*, unsigned long) at rtcp_receiver.cc:174
webrtc::RtpVideoSender::DeliverRtcp(unsigned char const*, unsigned long) at rtp_video_sender.cc:565
webrtc::internal::Call::DeliverRtcp(webrtc::MediaType, unsigned char const*, unsigned long) at call.cc:1352
cricket::WebRtcVideoChannel::OnRtcpReceived(rtc::CopyOnWriteBuffer, long) at webrtc_video_engine.cc:1519
cricket::BaseChannel::ProcessPacket(bool, rtc::CopyOnWriteBuffer const&, long) at channel.cc:542
```

### receive receiver report
```c++
webrtc::GoogCcNetworkController::OnTransportLossReport(webrtc::TransportLossReport) at goog_cc_network_control.cc:364
webrtc::RtpTransportControllerSend::OnReceivedRtcpReceiverReportBlocks(list<webrtc::RTCPReportBlock, allocator<webrtc::RTCPReportBlock> > const&, long) at rtp_transport_controller_send.cc:574
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::RtpTransportControllerSend::OnReceivedRtcpReceiverReport(list<webrtc::RTCPReportBlock, allocator<webrtc::RTCPReportBlock> > const&, long, long)::$_9>::Run()  webrtc::RtpTransportControllerSend::OnReceivedRtcpReceiverReport(list<webrtc::RTCPReportBlock, allocator<webrtc::RTCPReportBlock> > const&, long, long)::$_9::operator()() const at rtp_transport_controller_send.cc:397
```

# FEC
### encode fec
```c++
webrtc::ForwardErrorCorrection::EncodeFec(list<unique_ptr<webrtc::ForwardErrorCorrection::Packet, default_delete<webrtc::ForwardErrorCorrection::Packet> >, allocator<unique_ptr<webrtc::ForwardErrorCorrection::Packet, default_delete<webrtc::ForwardErrorCorrection::Packet> > > > const&, unsigned char, int, bool, webrtc::FecMaskType, list<webrtc::ForwardErrorCorrection::Packet*, allocator<webrtc::ForwardErrorCorrection::Packet*> >*) at forward_error_correction.cc:112
webrtc::UlpfecGenerator::AddRtpPacketAndGenerateFec(unsigned char const*, unsigned long, unsigned long) at ulpfec_generator.cc:170
webrtc::FlexfecSender::AddRtpPacketAndGenerateFec(webrtc::RtpPacketToSend const&) at flexfec_sender.cc:115
webrtc::RTPSenderVideo::SendVideoPacketWithFlexfec(unique_ptr<webrtc::RtpPacketToSend, default_delete<webrtc::RtpPacketToSend> >, webrtc::StorageType, bool) at rtp_sender_video.cc:335
webrtc::RTPSenderVideo::SendVideo(webrtc::VideoFrameType, signed char, unsigned int, long, unsigned char const*, unsigned long, webrtc::RTPFragmentationHeader const*, webrtc::RTPVideoHeader const*, long) at rtp_sender_video.cc:706
webrtc::RtpVideoSender::OnEncodedImage(webrtc::EncodedImage const&, webrtc::CodecSpecificInfo const*, webrtc::RTPFragmentationHeader const*) at rtp_video_sender.cc:428
webrtc::internal::VideoSendStreamImpl::OnEncodedImage(webrtc::EncodedImage const&, webrtc::CodecSpecificInfo const*, webrtc::RTPFragmentationHeader const*) at video_send_stream_impl.cc:631
non-virtual thunk to webrtc::internal::VideoSendStreamImpl::OnEncodedImage(webrtc::EncodedImage const&, webrtc::CodecSpecificInfo const*, webrtc::RTPFragmentationHeader const*) at video_send_stream_impl.cc:0
webrtc::VideoStreamEncoder::OnEncodedImage(webrtc::EncodedImage const&, webrtc::CodecSpecificInfo const*, webrtc::RTPFragmentationHeader const*) at video_stream_encoder.cc:1559
non-virtual thunk to webrtc::VideoStreamEncoder::OnEncodedImage(webrtc::EncodedImage const&, webrtc::CodecSpecificInfo const*, webrtc::RTPFragmentationHeader const*) at video_stream_encoder.cc:0
webrtc::LibvpxVp8Encoder::GetEncodedPartitions(webrtc::VideoFrame const&) at libvpx_vp8_encoder.cc:1093
webrtc::LibvpxVp8Encoder::Encode(webrtc::VideoFrame const&, vector<webrtc::VideoFrameType, allocator<webrtc::VideoFrameType> > const*) at libvpx_vp8_encoder.cc:1006
webrtc::VideoStreamEncoder::EncodeVideoFrame(webrtc::VideoFrame const&, long) at video_stream_encoder.cc:1450
webrtc::VideoStreamEncoder::MaybeEncodeVideoFrame(webrtc::VideoFrame const&, long) at video_stream_encoder.cc:1316
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::VideoStreamEncoder::OnFrame(webrtc::VideoFrame const&)::$_9>::Run()  webrtc::VideoStreamEncoder::OnFrame(webrtc::VideoFrame const&)::$_9::operator()() const at video_stream_encoder.cc:1051
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::VideoStreamEncoder::OnFrame(webrtc::VideoFrame const&)::$_9>::Run() at to_queued_task.h:32
```
> set protect method
```c++
void FecControllerDefault::SetProtectionMethod(bool enable_fec,
                                               bool enable_nack) {
  media_optimization::VCMProtectionMethodEnum method(media_optimization::kNone);
  if (enable_fec && enable_nack) {
    method = media_optimization::kNackFec;
  } else if (enable_nack) {
    method = media_optimization::kNack;
  } else if (enable_fec) {
    method = media_optimization::kFec;
  }
  CritScope lock(&crit_sect_);
  loss_prot_logic_->SetMethod(method);
}
```

> update fec parameter
```c++
// Get the bit cost of protection method, based on the amount of
// overhead data actually transmitted (including headers) the last
// second.
// Get the FEC code rate for Key frames (set to 0 when NA).
key_fec_params.fec_rate =
    loss_prot_logic_->SelectedMethod()->RequiredProtectionFactorK();
// Get the FEC code rate for Delta frames (set to 0 when NA).
delta_fec_params.fec_rate =
    loss_prot_logic_->SelectedMethod()->RequiredProtectionFactorD();
// The RTP module currently requires the same |max_fec_frames| for both
// key and delta frames.
delta_fec_params.max_fec_frames =
    loss_prot_logic_->SelectedMethod()->MaxFramesFec();
key_fec_params.max_fec_frames =
    loss_prot_logic_->SelectedMethod()->MaxFramesFec();

```

> generate fec packet
```c++
if (first_protected_packet) {
  // Write P, X, CC, M, and PT recovery fields.
  // Note that bits 0, 1, and 16 are overwritten in FinalizeFecHeaders.
  memcpy(&fec_packet->data[0], &media_packet->data[0], 2);
  // Write length recovery field. (This is a temporary location for
  // ULPFEC.)
  ByteWriter<uint16_t>::WriteBigEndian(&fec_packet->data[2],
                                       media_payload_length);
  // Write timestamp recovery field.
  memcpy(&fec_packet->data[4], &media_packet->data[4], 4);
  // Write payload.
  memcpy(&fec_packet->data[fec_header_size],
         &media_packet->data[kRtpHeaderSize], media_payload_length);
} else {
  XorHeaders(*media_packet, fec_packet);
  XorPayloads(*media_packet, media_payload_length, fec_header_size,
              fec_packet);
}
```


### decode fec
```c++
webrtc::ForwardErrorCorrection::DecodeFec(webrtc::ForwardErrorCorrection::ReceivedPacket const&, list<unique_ptr<webrtc::ForwardErrorCorrection::RecoveredPacket, default_delete<webrtc::ForwardErrorCorrection::RecoveredPacket> >, allocator<unique_ptr<webrtc::ForwardErrorCorrection::RecoveredPacket, default_delete<webrtc::ForwardErrorCorrection::RecoveredPacket> > > >*) at forward_error_correction.cc:716
webrtc::FlexfecReceiver::ProcessReceivedPacket(webrtc::ForwardErrorCorrection::ReceivedPacket const&) at flexfec_receiver.cc:151
webrtc::FlexfecReceiver::OnRtpPacket(webrtc::RtpPacketReceived const&) at flexfec_receiver.cc:78
webrtc::FlexfecReceiveStreamImpl::OnRtpPacket(webrtc::RtpPacketReceived const&) at flexfec_receive_stream_impl.cc:186
webrtc::RtpVideoStreamReceiver::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_video_stream_receiver.cc:370
webrtc::RtpDemuxer::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_demuxer.cc:177
webrtc::RtpStreamReceiverController::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_stream_receiver_controller.cc:54
webrtc::internal::Call::DeliverRtp(webrtc::MediaType, rtc::CopyOnWriteBuffer, long) at call.cc:1437
webrtc::internal::Call::DeliverPacket(webrtc::MediaType, rtc::CopyOnWriteBuffer, long) at call.cc:1461
cricket::WebRtcVideoChannel::OnPacketReceived(rtc::CopyOnWriteBuffer, long) at webrtc_video_engine.cc:1405
cricket::BaseChannel::ProcessPacket(bool, rtc::CopyOnWriteBuffer const&, long) at channel.cc:544
```
> recovery missing capability
```c++
int ForwardErrorCorrection::NumCoveredPacketsMissing(
    const ReceivedFecPacket& fec_packet) {
  int packets_missing = 0;
  for (const auto& protected_packet : fec_packet.protected_packets) {
    if (protected_packet->pkt == nullptr) {
      ++packets_missing;
      if (packets_missing > 1) {
        break;  // We can't recover more than one packet.
      }
    }
  }
  return packets_missing;
}
```

### decode rs fec
```c++
webrtc::RtpVideoStreamReceiver::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_video_stream_receiver.cc:328
webrtc::RtpDemuxer::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_demuxer.cc:177
webrtc::RtpStreamReceiverController::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_stream_receiver_controller.cc:54
webrtc::internal::Call::OnRecoveredPacket(unsigned char const*, unsigned long) at call.cc:1565
webrtc::RSfecReceiver::ProcessReceivedPacket(webrtc::RSForwardErrorCorrection::ReceivedPacket const&) at rsfec_receiver.cc:205
webrtc::RSfecReceiver::OnRtpPacket(webrtc::RtpPacketReceived const&) at rsfec_receiver.cc:69
webrtc::RSfecReceiveStreamImpl::OnRtpPacket(webrtc::RtpPacketReceived const&) at rsfec_receive_stream_impl.cc:176
webrtc::RtpVideoStreamReceiver::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_video_stream_receiver.cc:373
webrtc::RtpDemuxer::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_demuxer.cc:177
webrtc::RtpStreamReceiverController::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_stream_receiver_controller.cc:54
webrtc::internal::Call::DeliverRtp(webrtc::MediaType, rtc::CopyOnWriteBuffer, long) at call.cc:1505
webrtc::internal::Call::DeliverPacket(webrtc::MediaType, rtc::CopyOnWriteBuffer, long) at call.cc:1529
cricket::WebRtcVideoChannel::OnPacketReceived(rtc::CopyOnWriteBuffer, long) at webrtc_video_engine.cc:1454
cricket::BaseChannel::ProcessPacket(bool, rtc::CopyOnWriteBuffer const&, long) at channel.cc:544
```

### fec recovery and give back to receiver
```c++
webrtc::RSfecReceiver::OnRtpPacket(webrtc::RtpPacketReceived const&) at rsfec_receiver.cc:64
webrtc::RSfecReceiveStreamImpl::OnRtpPacket(webrtc::RtpPacketReceived const&) at rsfec_receive_stream_impl.cc:176
webrtc::RtpVideoStreamReceiver::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_video_stream_receiver.cc:371
webrtc::RtpDemuxer::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_demuxer.cc:177
webrtc::RtpStreamReceiverController::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_stream_receiver_controller.cc:54
webrtc::internal::Call::OnRecoveredPacket(unsigned char const*, unsigned long) at call.cc:1572
webrtc::RSfecReceiver::ProcessReceivedPacket(webrtc::RSForwardErrorCorrection::ReceivedPacket const&) at rsfec_receiver.cc:224
webrtc::RSfecReceiver::OnRtpPacket(webrtc::RtpPacketReceived const&) at rsfec_receiver.cc:73
webrtc::RSfecReceiveStreamImpl::OnRtpPacket(webrtc::RtpPacketReceived const&) at rsfec_receive_stream_impl.cc:176
webrtc::RtpVideoStreamReceiver::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_video_stream_receiver.cc:371
```

### encode RED packet
```c++
webrtc::AudioEncoderCopyRed::EncodeImpl(unsigned int, rtc::ArrayView<short const, -4711l>, rtc::BufferT<unsigned char, false>*) at audio_encoder_copy_red.cc:76
webrtc::AudioEncoder::Encode(unsigned int, rtc::ArrayView<short const, -4711l>, rtc::BufferT<unsigned char, false>*) at audio_encoder.cc:44
webrtc::AudioCodingModuleImpl::Add10MsData(webrtc::AudioFrame const&)  webrtc::AudioCodingModuleImpl::Encode(webrtc::AudioCodingModuleImpl::InputData const&) at audio_coding_module.cc:366
webrtc::AudioCodingModuleImpl::Add10MsData(webrtc::AudioFrame const&) at audio_coding_module.cc:456
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::voe::ChannelSend::ProcessAndEncodeAudio()::ProcessAndEncodeAudio>::Run()  webrtc::voe::ChannelSend::ProcessAndEncodeAudioOnTaskQueue(webrtc::AudioFrame*) at channel_send.cc:1146
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::voe::ChannelSend::ProcessAndEncodeAudio()::ProcessAndEncodeAudio>::Run()  webrtc::voe::ChannelSend::ProcessAndEncodeAudio(unique_ptr<webrtc::AudioFrame, default_delete<webrtc::AudioFrame> >)::ProcessAndEncodeAudio::operator()() at channel_send.cc:1102
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::voe::ChannelSend::ProcessAndEncodeAudio()::ProcessAndEncodeAudio>::Run() at to_queued_task.h:32
webrtc::TaskQueueLibevent::OnWakeup(int, short, void*) at task_queue_libevent.cc:295
```

### split RED packet
```c++
webrtc::RedPayloadSplitter::SplitRed(list<webrtc::Packet, allocator<webrtc::Packet> >*) at red_payload_splitter.cc:42
webrtc::NetEqImpl::InsertPacketInternal(webrtc::RTPHeader const&, rtc::ArrayView<unsigned char const, -4711l>, unsigned int) at neteq_impl.cc:539
webrtc::NetEqImpl::InsertPacket(webrtc::RTPHeader const&, rtc::ArrayView<unsigned char const, -4711l>, unsigned int) at neteq_impl.cc:154
webrtc::acm2::AcmReceiver::InsertPacket(webrtc::RTPHeader const&, rtc::ArrayView<unsigned char const, -4711l>) at acm_receiver.cc:117
webrtc::AudioCodingModuleImpl::IncomingPacket(unsigned char const*, unsigned long, webrtc::RTPHeader const&) at audio_coding_module.cc:693
webrtc::voe::ChannelReceive::OnReceivedPayloadData(unsigned char const*, unsigned long, webrtc::RTPHeader const&) at channel_receive.cc:277
webrtc::voe::ChannelReceive::OnRtpPacket(webrtc::RtpPacketReceived const&) webrtc::voe::ChannelReceive::ReceivePacket(unsigned char const*, unsigned long, webrtc::RTPHeader const&) at channel_receive.cc:0
webrtc::voe::ChannelReceive::OnRtpPacket(webrtc::RtpPacketReceived const&) at channel_receive.cc:614
webrtc::RtpDemuxer::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_demuxer.cc:177
webrtc::RtpStreamReceiverController::OnRtpPacket(webrtc::RtpPacketReceived const&) at rtp_stream_receiver_controller.cc:54
webrtc::internal::Call::DeliverRtp(webrtc::MediaType, rtc::CopyOnWriteBuffer, long) at call.cc:1423
webrtc::internal::Call::DeliverPacket(webrtc::MediaType, rtc::CopyOnWriteBuffer, long) at call.cc:1461
cricket::WebRtcVoiceMediaChannel::OnPacketReceived(rtc::CopyOnWriteBuffer, long) at webrtc_voice_engine.cc:2094
cricket::BaseChannel::ProcessPacket(bool, rtc::CopyOnWriteBuffer const&, long) at channel.cc:544
```
> debug basic packet info 
```c++
  GetRtpPayloadType(packet.cdata(), packet.size(), &pt);
```

> don't insert packet list when ts is the same
```c++
// The new packet is to be inserted to the right of |rit|. If it has the same
// timestamp as |rit|, which has a higher priority, do not insert the new
// packet to list.
if (rit != buffer_.rend() && packet.timestamp == rit->timestamp) {
  LogPacketDiscarded(packet.priority.codec_level, stats);
  return return_val;
}
```



e2e_delay_counte

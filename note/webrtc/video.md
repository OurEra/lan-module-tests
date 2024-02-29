# VIDEO CAPTURE AND ENCODE

### encoder as sink
```c++
rtc::VideoBroadcaster::AddOrUpdateSink(rtc::VideoSinkInterface<webrtc::VideoFrame>*, rtc::VideoSinkWants const&) at video_broadcaster.cc:37
rtc::AdaptedVideoTrackSource::AddOrUpdateSink(rtc::VideoSinkInterface<webrtc::VideoFrame>*, rtc::VideoSinkWants const&) at adapted_video_track_source.cc:71
webrtc::VideoTrack::AddOrUpdateSink(rtc::VideoSinkInterface<webrtc::VideoFrame>*, rtc::VideoSinkWants const&) at video_track.cc:48
webrtc::MethodCall2<webrtc::VideoTrackInterface, void, rtc::VideoSinkInterface<webrtc::VideoFrame>*, rtc::VideoSinkWants const&>::OnMessage(rtc::Message*) at proxy.h:252
webrtc::MethodCall2<webrtc::VideoTrackInterface, void, rtc::VideoSinkInterface<webrtc::VideoFrame>*, rtc::VideoSinkWants const&>::Marshal(rtc::Location const&, rtc::Thread*) at proxy.h:246
webrtc::VideoTrackProxyWithInternal<webrtc::VideoTrackInterface>::AddOrUpdateSink(rtc::VideoSinkInterface<webrtc::VideoFrame>*, rtc::VideoSinkWants const&) at media_stream_track_proxy.h:52
cricket::WebRtcVideoChannel::WebRtcVideoSendStream::AddOrUpdateSink(rtc::VideoSinkInterface<webrtc::VideoFrame>*, rtc::VideoSinkWants const&) at webrtc_video_engine.cc:2218
webrtc::VideoStreamEncoder::VideoSourceProxy::SetSource(rtc::VideoSourceInterface<webrtc::VideoFrame>*, webrtc::DegradationPreference const&) at video_stream_encoder.cc:254
webrtc::VideoStreamEncoder::SetSource(rtc::VideoSourceInterface<webrtc::VideoFrame>*, webrtc::DegradationPreference const&) at video_stream_encoder.cc:637
cricket::WebRtcVideoChannel::WebRtcVideoSendStream::SetVideoSend(cricket::VideoOptions const*, rtc::VideoSourceInterface<webrtc::VideoFrame>*) at webrtc_video_engine.cc:1818
cricket::WebRtcVideoChannel::SetVideoSend(unsigned int, cricket::VideoOptions const*, rtc::VideoSourceInterface<webrtc::VideoFrame>*) at webrtc_video_engine.cc:1043
rtc::FunctorMessageHandler<bool, webrtc::VideoRtpSender::SetSend()::$_11>::OnMessage(rtc::Message*) [inlined] webrtc::VideoRtpSender::SetSend()::$_11::operator()() const at rtp_sender.cc:601
rtc::FunctorMessageHandler<bool, webrtc::VideoRtpSender::SetSend()::$_11>::OnMessage(rtc::Message*) at message_handler.h:42
rtc::MessageQueue::Dispatch(rtc::Message*) at message_queue.cc:513
```

### capture deliver frame
```c++
webrtc::VideoStreamEncoder::OnFrame(webrtc::VideoFrame const&) at video_stream_encoder.cc:986
rtc::VideoBroadcaster::OnFrame(webrtc::VideoFrame const&) at video_broadcaster.cc:89
rtc::AdaptedVideoTrackSource::OnFrame(webrtc::VideoFrame const&) at adapted_video_track_source.cc:62
webrtc::jni::AndroidVideoTrackSource::OnFrameCaptured(_JNIEnv*, webrtc::JavaRef<_jobject*> const&, int, long, webrtc::JavaRef<_jobject*> const&) at android_video_track_source.cc:151
::Java_org_webrtc_NativeAndroidVideoTrackSource_nativeOnFrameCaptured() at NativeAndroidVideoTrackSource_jni.h:100
```

### frame to sink
```c++
webrtc::jni::VideoSinkWrapper::OnFrame(webrtc::VideoFrame const&) at video_sink.cc:27
rtc::VideoBroadcaster::OnFrame(webrtc::VideoFrame const&) at video_broadcaster.cc:89
rtc::AdaptedVideoTrackSource::OnFrame(webrtc::VideoFrame const&) at adapted_video_track_source.cc:62
webrtc::jni::AndroidVideoTrackSource::OnFrameCaptured(_JNIEnv*, webrtc::JavaRef<_jobject*> const&, int, long, webrtc::JavaRef<_jobject*> const&) at android_video_track_source.cc:151
::Java_org_webrtc_NativeAndroidVideoTrackSource_nativeOnFrameCaptured() at NativeAndroidVideoTrackSource_jni.h:100
```

### frame to encode
```c++
webrtc::jni::VideoEncoderWrapper::Encode(webrtc::VideoFrame const&, std::__1::vector<webrtc::VideoFrameType, allocator<webrtc::VideoFrameType> > const*) at video_encoder_wrapper.cc:166
webrtc::(anonymous namespace)::VideoEncoderSoftwareFallbackWrapper::Encode(webrtc::VideoFrame const&, std::__1::vector<webrtc::VideoFrameType, allocator<webrtc::VideoFrameType> > const*) at video_encoder_software_fallback_wrapper.cc:252
webrtc::VideoStreamEncoder::EncodeVideoFrame(webrtc::VideoFrame const&, long) at video_stream_encoder.cc:1444
webrtc::VideoStreamEncoder::MaybeEncodeVideoFrame(webrtc::VideoFrame const&, long) at video_stream_encoder.cc:1310
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::VideoStreamEncoder::OnFrame(webrtc::VideoFrame const&)::$_9>::Run() [inlined] webrtc::VideoStreamEncoder::OnFrame(webrtc::VideoFrame const&)::$_9::operator()() const at video_stream_encoder.cc:1045
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::VideoStreamEncoder::OnFrame(webrtc::VideoFrame const&)::$_9>::Run() at to_queued_task.h:32
webrtc::(anonymous namespace)::TaskQueueLibevent::OnWakeup(int, short, void*) at task_queue_libevent.cc:295
event_base_loop [inlined] event_process_active at event.c:381
event_base_loop at event.c:521
```

### encoded callback to rtp
```c++
webrtc::RTPSenderVideo::SendVideoPacket(std::__1::unique_ptr<webrtc::RtpPacketToSend, default_delete<webrtc::RtpPacketToSend> >, webrtc::StorageType) at rtp_sender_video.cc:261
webrtc::RTPSenderVideo::SendVideo(webrtc::VideoFrameType, signed char, unsigned int, long, unsigned char const*, unsigned long, webrtc::RTPFragmentationHeader const*, webrtc::RTPVideoHeader const*, long) at rtp_sender_video.cc:714
webrtc::RtpVideoSender::OnEncodedImage(webrtc::EncodedImage const&, webrtc::CodecSpecificInfo const*, webrtc::RTPFragmentationHeader const*) at rtp_video_sender.cc:428
webrtc::internal::VideoSendStreamImpl::OnEncodedImage(webrtc::EncodedImage const&, webrtc::CodecSpecificInfo const*, webrtc::RTPFragmentationHeader const*) at video_send_stream_impl.cc:631
webrtc::internal::VideoSendStreamImpl::OnEncodedImage(webrtc::EncodedImage const&, webrtc::CodecSpecificInfo const*, webrtc::RTPFragmentationHeader const*) at video_send_stream_impl.cc:0
webrtc::VideoStreamEncoder::OnEncodedImage(webrtc::EncodedImage const&, webrtc::CodecSpecificInfo const*, webrtc::RTPFragmentationHeader const*) at video_stream_encoder.cc:1553
webrtc::VideoStreamEncoder::OnEncodedImage(webrtc::EncodedImage const&, webrtc::CodecSpecificInfo const*, webrtc::RTPFragmentationHeader const*) at video_stream_encoder.cc:0
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::jni::VideoEncoderWrapper::OnEncodedFrame(_JNIEnv*, webrtc::JavaRef<_jobject*> const&, webrtc::JavaRef<_jobject*> const&, int, int, long, int, int, unsigned char, webrtc::JavaRef<_jobject*> const&, int)::$_1>::Run() [inlined] webrtc::jni::VideoEncoderWrapper::OnEncodedFrame(_JNIEnv*, webrtc::JavaRef<_jobject*> const&, webrtc::JavaRef<_jobject*> const&, int, int, long, int, int, unsigned char, webrtc::JavaRef<_jobject*> const&, int)::$_1::operator()() const at video_encoder_wrapper.cc:312
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::jni::VideoEncoderWrapper::OnEncodedFrame(_JNIEnv*, webrtc::JavaRef<_jobject*> const&, webrtc::JavaRef<_jobject*> const&, int, int, long, int, int, unsigned char, webrtc::JavaRef<_jobject*> const&, int)::$_1>::Run() at to_queued_task.h:32
```

> encoded fragment packetization
```c++
std::unique_ptr<RtpPacketizer> packetizer = RtpPacketizer::Create(
    video_type, rtc::MakeArrayView(payload_data, payload_size), limits,
    *packetize_video_header, frame_type, fragmentation);

const size_t num_packets = packetizer->NumPackets();
```
> two type packetization
```c++
if (fragment_len > single_packet_capacity) {
  if (!PacketizeFuA(i))
    return false;
  ++i;
} else {
  i = PacketizeStapA(i);
}
```

### adjust higher resolution
> adjust when QP low
```c++
webrtc::VideoStreamEncoder::VideoSourceProxy::RequestHigherResolutionThan(int) at video_stream_encoder.cc:359
webrtc::VideoStreamEncoder::AdaptUp(webrtc::AdaptationObserverInterface::AdaptReason) at video_stream_encoder.cc:1840
webrtc::QualityScaler::CheckQp() at quality_scaler.cc:189
webrtc::webrtc_repeating_task_impl::RepeatingTaskImpl<webrtc::QualityScaler::QualityScaler(rtc::TaskQueue*, webrtc::AdaptationObserverInterface*, webrtc::VideoEncoder::QpThresholds, long)::$_1>::RunClosure() [inlined] webrtc::QualityScaler::QualityScaler(rtc::TaskQueue*, webrtc::AdaptationObserverInterface*, webrtc::VideoEncoder::QpThresholds, long)::$_1::operator()() const at quality_scaler.cc:100
webrtc::webrtc_repeating_task_impl::RepeatingTaskImpl<webrtc::QualityScaler::QualityScaler(rtc::TaskQueue*, webrtc::AdaptationObserverInterface*, webrtc::VideoEncoder::QpThresholds, long)::$_1>::RunClosure() at repeating_task.h:64
webrtc::webrtc_repeating_task_impl::RepeatingTaskBase::Run() at repeating_task.cc:32
webrtc::(anonymous namespace)::TaskQueueLibevent::RunTimer(int, short, void*) at task_queue_libevent.cc:317
event_base_loop [inlined] event_process_active at event.c:381
event_base_loop at event.c:521
webrtc::(anonymous namespace)::TaskQueueLibevent::ThreadMain(void*) at task_queue_libevent.cc:266
```

> adjust step
```c++
// On step down we request at most 3/5 the pixel count of the previous
// resolution, so in order to take "one step up" we request a resolution
// as close as possible to 5/3 of the current resolution. The actual pixel
// count selected depends on the capabilities of the source. In order to
// not take a too large step up, we cap the requested pixel count to be at
// most four time the current number of pixels.
sink_wants_.target_pixel_count = (pixel_count * 5) / 3;
```

### set track source resolution
> set prefered resolution
```c++
cricket::VideoAdapter::OnOutputFormatRequest(absl::optional<std::__1::pair<int, int> > const&, absl::optional<int> const&, absl::optional<std::__1::pair<int, int> > const&, absl::optional<int> const&, absl::optional<int> const&) at video_adapter.cc:303
webrtc::jni::AndroidVideoTrackSource::AdaptOutputFormat(_JNIEnv*, webrtc::JavaRef<_jobject*> const&, int, int, webrtc::JavaRef<_jobject*> const&, int, int, webrtc::JavaRef<_jobject*> const&, webrtc::JavaRef<_jobject*> const&) at android_video_track_source.cc:168
Java_org_webrtc_NativeAndroidVideoTrackSource_nativeAdaptOutputFormat() at NativeAndroidVideoTrackSource_jni.h:68
```
> calculate resolution on frame
```c++
cricket::VideoAdapter::AdaptFrameResolution(int, int, long, int*, int*, int*, int*) at video_adapter.cc:164
rtc::AdaptedVideoTrackSource::AdaptFrame(int, int, long, int*, int*, int*, int*, int*, int*) at adapted_video_track_source.cc:109
webrtc::jni::AndroidVideoTrackSource::AdaptFrame(_JNIEnv*, webrtc::JavaRef<_jobject*> const&, int, int, int, long) at android_video_track_source.cc:0
::Java_org_webrtc_NativeAndroidVideoTrackSource_nativeAdaptFrame() at NativeAndroidVideoTrackSource_jni.h:86
```
> crop based on calculate
```java
public void onFrameCaptured(VideoFrame frame) {
  final VideoProcessor.FrameAdaptationParameters parameters =
      nativeAndroidVideoTrackSource.adaptFrame(frame);
  synchronized (videoProcessorLock) {
    if (videoProcessor != null) {
      videoProcessor.onFrameCaptured(frame, parameters);
      return;
    }
  }

  VideoFrame adaptedFrame = VideoProcessor.applyFrameAdaptationParameters(frame, parameters);
  if (adaptedFrame != null) {
    nativeAndroidVideoTrackSource.onFrameCaptured(adaptedFrame);
    adaptedFrame.release();
  }
}
```

# VIDEO DECODE AND RENDER
### VideoReceiver register callback to decoder
```c++
webrtc::jni::VideoDecoderWrapper::RegisterDecodeCompleteCallback(webrtc::DecodedImageCallback*) at video_decoder_wrapper.cc:130
webrtc::VCMDecoderDataBase::GetDecoder(webrtc::VCMEncodedFrame const&, webrtc::VCMDecodedFrameCallback*) at decoder_database.cc:123
webrtc::vcm::VideoReceiver::Decode(webrtc::VCMEncodedFrame const&) at video_receiver.cc:300
webrtc::internal::VideoReceiveStream::HandleEncodedFrame(unique_ptr<webrtc::video_coding::EncodedFrame, <webrtc::video_coding::EncodedFrame> >) at video_receive_stream.cc:681
VideoReceiveStream::StartNextDecode()::DecodeTask>::Run() [inlined] webrtc::internal::VideoReceiveStream::StartNextDecode()::DecodeTask::operator()() at video_receive_stream.cc:618
VideoReceiveStream::StartNextDecode()::DecodeTask>::Run() at to_queued_task.h:32
```

###  VideoStreamDecoder register callback to VideoReceiver
```c++
webrtc::vcm::VideoReceiver::RegisterReceiveCallback(webrtc::VCMReceiveCallback*) at video_receiver.cc:147
webrtc::internal::VideoReceiveStream::Start() at video_receive_stream.cc:386
cricket::WebRtcVideoChannel::WebRtcVideoReceiveStream::RecreateWebRtcVideoStream() at webrtc_video_engine.cc:2752
cricket::WebRtcVideoChannel::AddRecvStream(cricket::StreamParams const&, bool) at webrtc_video_engine.cc:1255
cricket::BaseChannel::UpdateRemoteStreams_w(vector<cricket::StreamParams, <cricket::StreamParams> > const&, webrtc::SdpType, basic_string<char, char_traits<char>, allocator<char> >*) at channel.cc:718
cricket::VideoChannel::SetRemoteContent_w(cricket::MediaContentDescription const*, webrtc::SdpType, basic_string<char, char_traits<char>, allocator<char> >*) at channel.cc:1095
```
> VideoReceiver's  _decodedFrameCallback will callback to VideoStreamDecoder::FrameToRender();
> VideoStreamDecoder then callback to IncomingVideoStream::OnFrame(), after process frame will be back to VideoReceiveStream::OnFrame()
> Since VideoReceiveStream::Config.renderer is set to WebRtcVideoChannel::WebRtcVideoReceiveStream, so frame will callback to WebRtcVideoReceiveStream::OnFrame()

# VIDEO WITH SIMULCAST
VideoSendStreamParameters
  webrtc::VideoSendStream::Config config
    RtpConfig rtp
      std::vector<uint32_t> ssrcs

WebRtcVideoChannel::WebRtcVideoSendStream::CreateVideoEncoderConfig()
  VideoEncoderConfig.number_of_streams


max_bitrate_bps/min_bitrate_bps/target_bitrate_bps

scale_resolution_down_by
num_temporal_layers

### default layers config
```c++
std::vector<webrtc::VideoStream> GetSimulcastConfig(
    size_t max_layers,
    int width,
    int height,
    int /*max_bitrate_bps*/,
    double bitrate_priority,
    int max_qp,
    int /*max_framerate*/,
    bool is_screenshare,
    bool temporal_layers_supported) {
  if (is_screenshare) {
    return GetScreenshareLayers(max_layers, width, height, bitrate_priority,
                                max_qp, ScreenshareSimulcastFieldTrialEnabled(),
                                temporal_layers_supported);
  } else {
    return GetNormalSimulcastLayers(max_layers, width, height, bitrate_priority,
                                    max_qp, temporal_layers_supported);
  }
}
```

### distribute bitrate per layer
```c++
webrtc::VideoBitrateAllocation::SetBitrate(unsigned long, unsigned long, unsigned int) at video_bitrate_allocation.cc:42
webrtc::SimulcastRateAllocator::DistributeAllocationToSimulcastLayers(unsigned int, webrtc::VideoBitrateAllocation*) at simulcast_rate_allocator.cc:159
webrtc::SimulcastRateAllocator::GetAllocation(unsigned int, unsigned int) at simulcast_rate_allocator.cc:69
webrtc::VideoStreamEncoder::UpdateBitrateAllocationAndNotifyObserver(webrtc::VideoStreamEncoder::EncoderRateSettings const&) at video_stream_encoder.cc:1113
webrtc::VideoStreamEncoder::OnBitrateUpdated(webrtc::DataRate, webrtc::DataRate, unsigned char, long) at video_stream_encoder.cc:1666
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::VideoStreamEncoder::OnBitrateUpdated(webrtc::DataRate, webrtc::DataRate, unsigned char, long>::Run() webrtc::VideoStreamEncoder::OnBitrateUpdated()::operator()() const at video_stream_encoder.cc:1627
```
> distribute from smallest to biggest
```c++
// Allocate up to the target bitrate for each active simulcast layer.
for (; active_layer < codec_.numberOfSimulcastStreams; ++active_layer) {
  const SimulcastStream& stream =
      codec_.simulcastStream[layer_index[active_layer]];
  if (!stream.active) {
    stream_enabled_[layer_index[active_layer]] = false;
    continue;
  }
  // If we can't allocate to the current layer we can't allocate to higher
  // layers because they require a higher minimum bitrate.
  uint32_t min_bitrate = stream.minBitrate * 1000;
  if (!first_allocation && !stream_enabled_[layer_index[active_layer]]) {
    min_bitrate = std::min(
        static_cast<uint32_t>(hysteresis_factor_ * min_bitrate + 0.5),
        stream.targetBitrate * 1000);
  }
  if (left_to_allocate < min_bitrate) {
    break;
  }

  // We are allocating to this layer so it is the current active allocation.
  top_active_layer = layer_index[active_layer];
  stream_enabled_[layer_index[active_layer]] = true;
  uint32_t allocation =
      std::min(left_to_allocate, stream.targetBitrate * 1000);
  allocated_bitrates_bps->SetBitrate(layer_index[active_layer], 0,
                                     allocation);
  RTC_DCHECK_LE(allocation, left_to_allocate);
  left_to_allocate -= allocation;
}
```


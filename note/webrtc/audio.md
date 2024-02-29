# AUDIO CAPTURE AND ENCODE

### start audio device record
```c++
webrtc::jni::AndroidAudioDeviceModule::StartRecording() at audio_device_module.cc:281
webrtc::internal::AudioState::AddSendingStream(webrtc::AudioSendStream*, int, unsigned long) at audio_state.cc:103
webrtc::internal::AudioSendStream::Start() at audio_send_stream.cc:344
cricket::WebRtcVoiceMediaChannel::SetSend(bool) at webrtc_voice_engine.cc:1791
cricket::VoiceChannel::UpdateMediaSendRecvState_w() at channel.cc:945
cricket::VoiceChannel::SetRemoteContent_w(cricket::MediaContentDescription const*, webrtc::SdpType, basic_string<char, char_traits<char>, allocator<char> >*) at channel.cc:1043
```

### stop audio device record
```c++
webrtc::jni::AndroidAudioDeviceModule::StopRecording() at audio_device_module.cc:300
webrtc::internal::AudioState::RemoveSendingStream(webrtc::AudioSendStream*) at audio_state.cc:117
cricket::WebRtcVoiceMediaChannel::SetLocalSource(unsigned int, cricket::AudioSource*) at webrtc_voice_engine.cc:1976
cricket::WebRtcVoiceMediaChannel::SetAudioSend(unsigned int, bool, cricket::AudioOptions const*, cricket::AudioSource*) at webrtc_voice_engine.cc:1804
rtc::FunctorMessageHandler<bool, webrtc::AudioRtpSender::ClearSend()::$_10>::OnMessage(rtc::Message*) [inlined] webrtc::AudioRtpSender::ClearSend()::$_10::operator()() const at rtp_sender.cc:533
```

### set send codec
```c++
cricket::WebRtcVoiceMediaChannel::SetSendCodecs(vector<cricket::AudioCodec, allocator<cricket::AudioCodec> > const&) at webrtc_voice_engine.cc:1606
cricket::WebRtcVoiceMediaChannel::SetSendParameters(cricket::AudioSendParameters const&) at webrtc_voice_engine.cc:1301
cricket::VoiceChannel::SetRemoteContent_w(cricket::MediaContentDescription const*, webrtc::SdpType, basic_string<char, char_traits<char>, allocator<char> >*) at channel.cc:941
webrtc::internal::AudioSendStream::SetupSendCodec(webrtc::internal::AudioSendStream*, webrtc::AudioSendStream::Config const&) [inlined] absl::optional<webrtc::AudioSendStream::Config::SendCodecSpec>::operator bool() const at optional.h:815
webrtc::internal::AudioSendStream::SetupSendCodec(webrtc::internal::AudioSendStream*, webrtc::AudioSendStream::Config const&) at audio_send_stream.cc:573
webrtc::internal::AudioSendStream::ConfigureStream(webrtc::internal::AudioSendStream*, webrtc::AudioSendStream::Config const&, bool) at audio_send_stream.cc:310
webrtc::internal::AudioSendStream::AudioSendStream(webrtc::Clock*, webrtc::AudioSendStream::Config const&, rtc::scoped_refptr<webrtc::AudioState> const&, webrtc::TaskQueueFactory*, webrtc::RtpTransportControllerSendInterface*, webrtc::BitrateAllocatorInterface*, webrtc::RtcEventLog*, webrtc::RtcpRttStats*, absl::optional<webrtc::RtpState> const&, unique_ptr<webrtc::voe::ChannelSendInterface, std::default_delete<webrtc::voe::ChannelSendInterface> >) at audio_send_stream.cc:167
webrtc::internal::AudioSendStream::AudioSendStream(webrtc::Clock*, webrtc::AudioSendStream::Config const&, rtc::scoped_refptr<webrtc::AudioState> const&, webrtc::TaskQueueFactory*, webrtc::ProcessThread*, webrtc::RtpTransportControllerSendInterface*, webrtc::BitrateAllocatorInterface*, webrtc::RtcEventLog*, webrtc::RtcpRttStats*, absl::optional<webrtc::RtpState> const&) at audio_send_stream.cc:95
webrtc::internal::Call::CreateAudioSendStream(webrtc::AudioSendStream::Config const&) at call.cc:726
cricket::WebRtcVoiceMediaChannel::WebRtcAudioSendStream::WebRtcAudioSendStream(unsigned int, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, absl::optional<webrtc::AudioSendStream::Config::SendCodecSpec> const&, bool, std::vector<webrtc::RtpExtension, allocator<webrtc::RtpExtension> > const&, int, int, absl::optional<basic_string<char, char_traits<char>, allocator<char> > > const&, webrtc::Call*, webrtc::Transport*, webrtc::MediaTransportInterface*, rtc::scoped_refptr<webrtc::AudioEncoderFactory> const&, absl::optional<webrtc::AudioCodecPairId>, rtc::scoped_refptr<webrtc::FrameEncryptorInterface>, webrtc::CryptoOptions const&) at webrtc_voice_engine.cc:745
cricket::WebRtcVoiceMediaChannel::AddSendStream(cricket::StreamParams const&) at webrtc_voice_engine.cc:1813
cricket::BaseChannel::UpdateLocalStreams_w(vector<cricket::StreamParams, allocator<cricket::StreamParams> > const&, webrtc::SdpType, std::basic_string<char, std::char_traits<char>, std::allocator<char> >*) at channel.cc:675
cricket::VoiceChannel::SetLocalContent_w(cricket::MediaContentDescription const*, webrtc::SdpType, basic_string<char, char_traits<char>, allocator<char> >*) at channel.cc:908
```

### record to send
```c++
webrtc::voe::ChannelSend::ProcessAndEncodeAudio(unique_ptr<webrtc::AudioFrame, default_delete<webrtc::AudioFrame> >) at channel_send.cc:1098
webrtc::internal::AudioSendStream::SendAudioData(unique_ptr<webrtc::AudioFrame, default_delete<webrtc::AudioFrame> >) at audio_send_stream.cc:361
webrtc::AudioTransportImpl::RecordedDataIsAvailable(void const*, unsigned long, unsigned long, unsigned long, unsigned int, unsigned int, int, unsigned int, bool, unsigned int&) at audio_transport_impl.cc:170
webrtc::AudioDeviceBuffer::DeliverRecordedData() at audio_device_buffer.cc:270
webrtc::jni::AudioRecordJni::DataIsRecorded(_JNIEnv*, webrtc::JavaParamRef<_jobject*> const&, int) at audio_record_jni.cc:248
```
> AudioTransport connect with audio device module
```c++
// Connect the ADM to our audio path.
adm()->RegisterAudioCallback(audio_state()->audio_transport());
```


### send to encode
```c++
webrtc::AudioEncoderOpusImpl::EncodeImpl(unsigned int, rtc::ArrayView<short const, -4711l>, rtc::BufferT<unsigned char, false>*) at audio_encoder_opus.cc:709
webrtc::AudioEncoder::Encode(unsigned int, rtc::ArrayView<short const, -4711l>, rtc::BufferT<unsigned char, false>*) at audio_encoder.cc:44
webrtc::AudioCodingModuleImpl::Add10MsData(webrtc::AudioFrame const&) [inlined] webrtc::AudioCodingModuleImpl::Encode(webrtc::AudioCodingModuleImpl::InputData const&) at audio_coding_module.cc:366
webrtc::AudioCodingModuleImpl::Add10MsData(webrtc::AudioFrame const&) at audio_coding_module.cc:456
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::voe::ChannelSend::ProcessAndEncodeAudio(unique_ptr<webrtc::AudioFrame, default_delete<webrtc::AudioFrame> >)::ProcessAndEncodeAudio>::Run() webrtc::voe::ChannelSend::ProcessAndEncodeAudioOnTaskQueue(webrtc::AudioFrame*) at channel_send.cc:1146
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::voe::ChannelSend::ProcessAndEncodeAudio(unique_ptr<webrtc::AudioFrame, default_delete<webrtc::AudioFrame> >)::ProcessAndEncodeAudio>::Run() webrtc::voe::ChannelSend::ProcessAndEncodeAudio(unique_ptr<webrtc::AudioFrame, default_delete<webrtc::AudioFrame> >)::ProcessAndEncodeAudio::operator()() at channel_send.cc:1102
webrtc::webrtc_new_closure_impl::ClosureTask<webrtc::voe::ChannelSend::ProcessAndEncodeAudio(unique_ptr<webrtc::AudioFrame, default_delete<webrtc::AudioFrame> >)::ProcessAndEncodeAudio>::Run() at to_queued_task.h:32
webrtc::TaskQueueLibevent::OnWakeup(int, short, void*) at task_queue_libevent.cc:295
```
> after encode, AudioEncoder::EncodedInfo.encoded_timestamp will be assign with first buffer timestamp that send to encoder

### encoder receive bwe (after distribute)
```c++
webrtc::AudioEncoderOpusImpl::OnReceivedUplinkAllocation(webrtc::BitrateAllocationUpdate) at audio_encoder_opus.cc:630
void rtc::FunctionView<void (webrtc::AudioEncoder*)>::CallVoidPtr<webrtc::voe::ChannelSend::OnBitrateAllocation(webrtc::BitrateAllocationUpdate)::$_4>(rtc::FunctionView<void (webrtc::AudioEncoder*)>::VoidUnion, webrtc::AudioEncoder*) [inlined] webrtc::voe::ChannelSend::OnBitrateAllocation(webrtc::BitrateAllocationUpdate)::$_4::operator()(webrtc::AudioEncoder*) const at channel_send.cc:831
void rtc::FunctionView<void (webrtc::AudioEncoder*)>::CallVoidPtr<webrtc::voe::ChannelSend::OnBitrateAllocation(webrtc::BitrateAllocationUpdate)::$_4>(rtc::FunctionView<void (webrtc::AudioEncoder*)>::VoidUnion, webrtc::AudioEncoder*) at function_view.h:109
webrtc::AudioCodingModuleImpl::ModifyEncoder(rtc::FunctionView<void (unique_ptr<webrtc::AudioEncoder, default_delete<webrtc::AudioEncoder> >*)>) at audio_coding_module.cc:432
webrtc::voe::ChannelSend::CallEncoder(rtc::FunctionView<void (webrtc::AudioEncoder*)>) at channel_send.cc:812
webrtc::voe::ChannelSend::OnBitrateAllocation(webrtc::BitrateAllocationUpdate) at channel_send.cc:830
webrtc::internal::AudioSendStream::OnBitrateUpdated(webrtc::BitrateAllocationUpdate) at audio_send_stream.cc:456
non-virtual thunk to webrtc::internal::AudioSendStream::OnBitrateUpdated(webrtc::BitrateAllocationUpdate) at audio_send_stream.cc:0
webrtc::BitrateAllocator::OnNetworkChanged(unsigned int, unsigned int, unsigned char, long, long) at bitrate_allocator.cc:131
webrtc::internal::Call::OnTargetTransferRate(webrtc::TargetTransferRate) at call.cc:1286
webrtc::RtpTransportControllerSend::UpdateControlState() at rtp_transport_controller_send.cc:146
webrtc::RtpTransportControllerSend::PostUpdates(webrtc::NetworkControlUpdate) at rtp_transport_controller_send.cc:530
webrtc::RtpTransportControllerSend::UpdateControllerWithTimeInterval() at rtp_transport_controller_send.cc:504
webrtc::webrtc_repeating_task_impl::RepeatingTaskImpl<webrtc::RtpTransportControllerSend::StartProcessPeriodicTasks()::$_13>::RunClosure() [inlined] webrtc::RtpTransportControllerSend::StartProcessPeriodicTasks()::$_13::operator()() const at rtp_transport_controller_send.cc:492
webrtc::webrtc_repeating_task_impl::RepeatingTaskImpl<webrtc::RtpTransportControllerSend::StartProcessPeriodicTasks()::$_13>::RunClosure() at repeating_task.h:64
```

# AUDIO 3A PROCESS

### process render data
```c++
audio_transport_impl.cc
int32_t AudioTransportImpl::NeedMorePlayData(const size_t nSamples,
                                             const size_t nBytesPerSample,
                                             const size_t nChannels,
                                             const uint32_t samplesPerSec,
                                             void* audioSamples,
                                             size_t& nSamplesOut,
                                             int64_t* elapsed_time_ms,
                                             int64_t* ntp_time_ms) {

  mixer_->Mix(nChannels, &mixed_frame_);
  *elapsed_time_ms = mixed_frame_.elapsed_time_ms_;
  *ntp_time_ms = mixed_frame_.ntp_time_ms_;

  const auto error = audio_processing_->ProcessReverseStream(&mixed_frame_);
  RTC_DCHECK_EQ(error, AudioProcessing::kNoError);

```

### process capture data
```c++
audio_transport_impl.cc
int32_t AudioTransportImpl::RecordedDataIsAvailable(
    const void* audio_data,
    const size_t number_of_frames,
    const size_t bytes_per_sample,
    const size_t number_of_channels,
    const uint32_t sample_rate,
    const uint32_t audio_delay_milliseconds,
    const int32_t /*clock_drift*/,
    const uint32_t /*volume*/,
    const bool key_pressed,
    uint32_t& /*new_mic_volume*/) {  // NOLINT: to avoid changing APIs

  InitializeCaptureFrame(sample_rate, send_sample_rate_hz, number_of_channels,
                         send_num_channels, audio_frame.get());
  voe::RemixAndResample(static_cast<const int16_t*>(audio_data),
                        number_of_frames, number_of_channels, sample_rate,
                        &capture_resampler_, audio_frame.get());
  ProcessCaptureFrame(audio_delay_milliseconds, key_pressed,
                      swap_stereo_channels, audio_processing_,
                      audio_frame.get());

```

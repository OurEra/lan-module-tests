## RTC 播放远端音频

当播放音频时，通过 NeedMorePlayData 接口获取当前音频帧

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

}
```

由于各个远端音频会注册一个 AudioMixer::Source 到 AudioMixerImpl，mix 则负责将所有的远端音频 Stream 混音输出
```c++
void AudioState::AddReceivingStream(webrtc::AudioReceiveStream* stream) {
  RTC_DCHECK(thread_checker_.IsCurrent());
  RTC_DCHECK_EQ(0, receiving_streams_.count(stream));
  receiving_streams_.insert(stream);
  if (!config_.audio_mixer->AddSource(
          static_cast<internal::AudioReceiveStream*>(stream))) {
    RTC_DLOG(LS_ERROR) << "Failed to add source to mixer.";
  }
}
```

其中，混音输出的采样率由以下逻辑决定

```c++
void AudioMixerImpl::CalculateOutputFrequency() {
  RTC_DCHECK_RUNS_SERIALIZED(&race_checker_);
  rtc::CritScope lock(&crit_);

  std::vector<int> preferred_rates;
  std::transform(audio_source_list_.begin(), audio_source_list_.end(),
                 std::back_inserter(preferred_rates),
                 [&](std::unique_ptr<SourceStatus>& a) {
                   return a->audio_source->PreferredSampleRate();
                 });

  output_frequency_ =
      output_rate_calculator_->CalculateOutputRate(preferred_rates);
  sample_size_ = (output_frequency_ * kFrameDurationInMs) / 1000;
}
```
在获取各个 source 的 prefer 采样率后，在 DefaultOutputRateCalculator::CalculateOutputRate() 处理
选取最大的采样率使用。

获取各个 Stream 的当前 pcm 数据在 GetAudioFromSources 中实现
```c++
AudioFrameList AudioMixerImpl::GetAudioFromSources() {
  RTC_DCHECK_RUNS_SERIALIZED(&race_checker_);
  AudioFrameList result;
  std::vector<SourceFrame> audio_source_mixing_data_list;
  std::vector<SourceFrame> ramp_list;

  // Get audio from the audio sources and put it in the SourceFrame vector.
  for (auto& source_and_status : audio_source_list_) {
    const auto audio_frame_info =
        source_and_status->audio_source->GetAudioFrameWithInfo(
            OutputFrequency(), &source_and_status->audio_frame);
 ...
}
```

由于 GetAudioFrameWithInfo 每次获取的音频数据为 10ms，因此实际上 mix 模块每次混音的数据也为 10ms：
```c++
AudioMixer::Source::AudioFrameInfo ChannelReceive::GetAudioFrameWithInfo(
    int sample_rate_hz,
    AudioFrame* audio_frame) {
  RTC_DCHECK_RUNS_SERIALIZED(&audio_thread_race_checker_);
  audio_frame->sample_rate_hz_ = sample_rate_hz;

  event_log_->Log(absl::make_unique<RtcEventAudioPlayout>(remote_ssrc_));

  // Get 10ms raw PCM data from the ACM (mixer limits output frequency)
  bool muted;
  if (audio_coding_->PlayoutData10Ms(audio_frame->sample_rate_hz_, audio_frame,
                                     &muted) == -1) {

 ...
}
```

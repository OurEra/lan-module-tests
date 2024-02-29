## RTC 创建音频 receive 对象

```c++
void PeerConnection::CreateAudioReceiver(
    MediaStreamInterface* stream,
    const RtpSenderInfo& remote_sender_info) {
  std::vector<rtc::scoped_refptr<MediaStreamInterface>> streams;
  streams.push_back(rtc::scoped_refptr<MediaStreamInterface>(stream));
  // TODO(https://crbug.com/webrtc/9480): When we remove remote_streams(), use
  // the constructor taking stream IDs instead.
  auto* audio_receiver = new AudioRtpReceiver(
      worker_thread(), remote_sender_info.sender_id, streams);
  audio_receiver->SetMediaChannel(voice_media_channel());
  audio_receiver->SetupMediaChannel(remote_sender_info.first_ssrc);
  auto receiver = RtpReceiverProxyWithInternal<RtpReceiverInternal>::Create(
      signaling_thread(), audio_receiver);
  GetAudioTransceiver()->internal()->AddReceiver(receiver);
  Observer()->OnAddTrack(receiver, streams);
  NoteUsageEvent(UsageEvent::AUDIO_ADDED);
}
```

创建 AudioRtpReceiver 并通过 SetMediaChannel() 设置对应在 voice engine 中的 WebRtcVoiceMediaChannel;
> AudioRtpReceiver.source_ 为 RemoteAudioSource 对象, 用户获取 source 后的操作通过 RemoteAudioSource.audio_observers_
再回到 AudioRtpReceiver 来处理；
> AudioRtpReceiver.track_ 为 AudioTrack 对象，用户获取 track 后可以设置音频回调
### 设置远端音量
使用方式:
> AudioTrackInterface.GetSource().SetVolume(volume)

内部流程为 AudioTrack 返回 RemoteAudioSource, 调用其 SetVolume() 方法; 流程到达 AudioRtpReceiver::OnSetVolume(),
然后到达 WebRtcVoiceMediaChannel::SetOutputVolume(), 再调用到 AudioReceiveStream::SetGain(), 最后到达
ChannelReceive::SetChannelOutputVolumeScaling()
### 设置远端音频回调
使用方式:
> AudioTrackInterface.AddSink()

内部流程为 AudioTrack::AddSink() 调用到 RemoteAudioSource::AddSink(), 由于 RemoteAudioSource 通过
WebRtcVoiceMediaChannel::SetRawAudioSink() 设置了音频回调, 所以当 RemoteAudioSource::OnData() 时,
数据就将回调到外面

## RTC 创建音频 send 对象
当用户创建完本地 Track 后, 调用 AddTrack()
> PeerConnectionFactory.CreateLocalMediaStream()
> MediaStream.AddTrack()

触发 PeerConnection 创建 send 对象
```c++
void PeerConnection::OnAudioTrackAdded(AudioTrackInterface* track,
                                       MediaStreamInterface* stream) {
  if (IsClosed()) {
    return;
  }
  AddAudioTrack(track, stream);
  UpdateNegotiationNeeded();
}

void PeerConnection::AddAudioTrack(AudioTrackInterface* track,
                                   MediaStreamInterface* stream) {
  RTC_DCHECK(!IsClosed());
  RTC_DCHECK(track);
  RTC_DCHECK(stream);
  auto sender = FindSenderForTrack(track);
  if (sender) {
    // We already have a sender for this track, so just change the stream_id
    // so that it's correct in the next call to CreateOffer.
    sender->internal()->set_stream_ids({stream->id()});
    return;
  }

  // Normal case; we've never seen this track before.
  auto new_sender = CreateSender(cricket::MEDIA_TYPE_AUDIO, track->id(), track,
                                 {stream->id()}, {});
  new_sender->internal()->SetMediaChannel(voice_media_channel());
...
}

rtc::scoped_refptr<RtpSenderProxyWithInternal<RtpSenderInternal>>
PeerConnection::CreateSender(
    cricket::MediaType media_type,
    const std::string& id,
    rtc::scoped_refptr<MediaStreamTrackInterface> track,
    const std::vector<std::string>& stream_ids,
    const std::vector<RtpEncodingParameters>& send_encodings) {
  RTC_DCHECK_RUN_ON(signaling_thread());
  rtc::scoped_refptr<RtpSenderProxyWithInternal<RtpSenderInternal>> sender;
  if (media_type == cricket::MEDIA_TYPE_AUDIO) {
    RTC_DCHECK(!track ||
               (track->kind() == MediaStreamTrackInterface::kAudioKind));
    sender = RtpSenderProxyWithInternal<RtpSenderInternal>::Create(
        signaling_thread(),
        AudioRtpSender::Create(worker_thread(), id, stats_.get()));
    NoteUsageEvent(UsageEvent::AUDIO_ADDED);
...

  bool set_track_succeeded = sender->SetTrack(track);
}
```

AudioRtpSender 通过接口 RtpSenderBase::SetTrack() 和 RtpSenderBase::SetMediaChannel() 持有对应的
AudioTrack 和 WebRtcVoiceMediaChannel


### 设置音频 FrameEncryptorInterface
使用方式:
> RtpSenderInterface.SetFrameEncryptor()

内部过程为 RtpSenderBase::SetFrameEncryptor() 调用 WebRtcVoiceMediaChannel::SetFrameEncryptor(), 然后到
AudioSendStream::Reconfigure(), 最后到 ChannelSend::SetFrameEncryptor()



## SDP 信息的传递

当 Remote SDP 设置后, 转换成 VideoSendParameters 结构并设置给 WebRtcVideoChannel::SetSendParameters()

```c++
bool VideoChannel::SetRemoteContent_w(const MediaContentDescription* content,
                                      SdpType type,
                                      std::string* error_desc) {
  ...
  VideoSendParameters send_params = last_send_params_;
  RtpSendParametersFromMediaDescription(video, rtp_header_extensions,
                                        &send_params);
  if (video->conference_mode()) {
    send_params.conference_mode = true;
  }
  send_params.mid = content_name();

  bool parameters_applied = media_channel()->SetSendParameters(send_params);
  ...

  return true;
}
```

此时, 获取 VideoCodecSettings.codec.params 时就可以获取 SDP 里面的相关设置
```sh
k level-asymmetry-allowed v 1
k packetization-mode v 1
k profile-level-id v 640c1f
```

```c++
bool WebRtcVideoChannel::SetSendParameters(const VideoSendParameters& params) {
  ChangedSendParameters changed_params;
  if (!GetChangedSendParameters(params, &changed_params)) {
    return false;
  }

  if (changed_params.codec) {
    const VideoCodecSettings& codec_settings = *changed_params.codec;
    send_codec_ = codec_settings;
    RTC_LOG(LS_INFO) << "Using codec: " << codec_settings.codec.ToString();
    for (auto& p : codec_settings.codec.params)
      RTC_LOG(INFO) << "k " << p.first << " v " << p.second;
  }
  ...
}
```

最后, 给到编码模块时, 将会转成 VideoEncoderConfig 结构传递; VideoEncoderConfig.video_format.parameters 中就是 sdp 相关信息

```c++
webrtc::VideoEncoderConfig
WebRtcVideoChannel::WebRtcVideoSendStream::CreateVideoEncoderConfig(
    const VideoCodec& codec) const {
  RTC_DCHECK_RUN_ON(&thread_checker_);
  webrtc::VideoEncoderConfig encoder_config;
  encoder_config.codec_type = webrtc::PayloadStringToCodecType(codec.name);
  encoder_config.video_format =
      webrtc::SdpVideoFormat(codec.name, codec.params);
  ...
}
```

## PROFILE 的判定

根据协议规范 sdp 描述 H264 profile& level 方式
> https://www.rfc-editor.org/rfc/rfc6184#section-8.1

按照规范, PROFILE 如下组织
```c++
// This is from https://tools.ietf.org/html/rfc6184#section-8.1.
constexpr ProfilePattern kProfilePatterns[] = {
    {0x42, BitPattern("x1xx0000"), kProfileConstrainedBaseline},
    {0x4D, BitPattern("1xxx0000"), kProfileConstrainedBaseline},
    {0x58, BitPattern("11xx0000"), kProfileConstrainedBaseline},
    {0x42, BitPattern("x0xx0000"), kProfileBaseline},
    {0x58, BitPattern("10xx0000"), kProfileBaseline},
    {0x4D, BitPattern("0x0x0000"), kProfileMain},
    {0x64, BitPattern("00000000"), kProfileHigh},
    {0x64, BitPattern("00001100"), kProfileConstrainedHigh}};
```

如果需要判定 PROFILE 是否一致, 可以使用函数按如下方式使用
```c++
bool IsSameH264Profile(const CodecParameterMap& params1,
                       const CodecParameterMap& params2);
```
```c++
H264::CodecParameterMap constrained_high;
constrained_high.emplace("profile-level-id", "640c1f");

H264::CodecParameterMap high;
high.emplace("profile-level-id", "640032");
if (H264::IsSameH264Profile(encoder_config_.video_format.parameters, constrained_high))
  RTC_LOG(INFO) << "is constrained_high";

if (H264::IsSameH264Profile(encoder_config_.video_format.parameters, high))
  RTC_LOG(INFO) << "is high";
```

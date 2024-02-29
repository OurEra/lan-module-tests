# CreateOffer
### sdk MediaConstraints to RTCOfferAnswerOptions
```c++
JNI_PeerConnection_CreateOffer {
  PeerConnectionInterface::RTCOfferAnswerOptions options;
  CopyConstraintsIntoOfferAnswerOptions(observer->constraints(), &options);
...
}

CopyConstraintsIntoOfferAnswerOptions {
...
   if (FindConstraint(constraints, MediaConstraints::kNumSimulcastLayers,
                      &layers, &mandatory_constraints_satisfied)) {
     offer_answer_options->num_simulcast_layers = layers;
   }
...
}
```

### prepare MediaSessionOptions
```c++
GetOptionsForPlanBAnswer {
...
  cricket::MediaDescriptionOptions* audio_media_description_options =
      !audio_index ? nullptr
                   : &session_options->media_description_options[*audio_index];
  cricket::MediaDescriptionOptions* video_media_description_options =
      !video_index ? nullptr
                   : &session_options->media_description_options[*video_index];

  AddPlanBRtpSenderOptions(GetSendersInternal(),
                           audio_media_description_options,
                           video_media_description_options,
                           offer_answer_options.num_simulcast_layers);

...
}
```

### generate SessionDescription
```c++
std::unique_ptr<SessionDescription> MediaSessionDescriptionFactory::CreateOffer(
    const MediaSessionOptions& session_options,
    const SessionDescription* current_description) const {
...
      case MEDIA_TYPE_AUDIO:
        if (!AddAudioContentForOffer(
                media_description_options, session_options, current_content,
                current_description, audio_rtp_extensions, offer_audio_codecs,
                &current_streams, offer.get(), &ice_credentials)) {
          return nullptr;
        }
        break;
      case MEDIA_TYPE_VIDEO:
        if (!AddVideoContentForOffer(
                media_description_options, session_options, current_content,
                current_description, video_rtp_extensions, offer_video_codecs,
                &current_streams, offer.get(), &ice_credentials)) {
          return nullptr;
        }
        break;
...
}
```

### SessionDescription to sdp string
```c++
JsepSessionDescription::JsepSessionDescription(
    SdpType type,
    std::unique_ptr<cricket::SessionDescription> description,
    absl::string_view session_id,
    absl::string_view session_version)
    : description_(std::move(description)),
      session_id_(session_id),
      session_version_(session_version),
      type_(type) {
  RTC_DCHECK(description_);
  candidate_collection_.resize(number_of_mediasections());
}

bool JsepSessionDescription::ToString(std::string* out) const {
  if (!description_ || !out) {
    return false;
  }
  *out = SdpSerialize(*this);
  return !out->empty();
}
```

### build ssrc-group
```c++
webrtc::BuildMediaDescription(cricket::ContentInfo const*, cricket::TransportInfo const*, cricket::MediaType, std::vector<cricket::Candidate, std::allocator<cricket::Candidate> > const&, int, std::basic_string<char, std::char_traits<char>, std::allocator<char> >*) [inlined] webrtc::BuildRtpContentAttributes(cricket::MediaContentDescription const*, cricket::MediaType, int, std::basic_string<char, std::char_traits<char>, std::allocator<char> >*) at webrtc_sdp.cc:1681
webrtc::BuildMediaDescription(cricket::ContentInfo const*, cricket::TransportInfo const*, cricket::MediaType, std::vector<cricket::Candidate, std::allocator<cricket::Candidate> > const&, int, std::basic_string<char, std::char_traits<char>, std::allocator<char> >*) at webrtc_sdp.cc:1530
webrtc::SdpSerialize(webrtc::JsepSessionDescription const&) at webrtc_sdp.cc:949
webrtc::JsepSessionDescription::ToString(std::basic_string<char, std::char_traits<char>, std::allocator<char> >*) const at jsep_session_description.cc:284
webrtc::jni::NativeToJavaSessionDescription(_JNIEnv*, webrtc::SessionDescriptionInterface const*) at session_description.cc:42
webrtc::jni::CreateSdpObserverJni::OnSuccess(webrtc::SessionDescriptionInterface*) at sdp_observer.cc:35
webrtc::WebRtcSessionDescriptionFactory::OnMessage(rtc::Message*) at webrtc_session_description_factory.cc:299
```

### generate ssrc
```c++
cricket::StreamParams::GenerateSsrcs(int, bool, bool, rtc::UniqueRandomIdGenerator*) at stream_params.cc:151
cricket::CreateStreamParamsForNewSenderWithSsrcs(cricket::SenderOptions const&, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, bool, bool, rtc::UniqueRandomIdGenerator*) at media_session.cc:409
bool cricket::AddStreamParams<cricket::AudioCodec>(std::vector<cricket::SenderOptions, std::allocator<cricket::SenderOptions> > const&, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, rtc::UniqueRandomIdGenerator*, std::vector<cricket::StreamParams, std::allocator<cricket::StreamParams> >*, cricket::MediaContentDescriptionImpl<cricket::AudioCodec>*) at media_session.cc:513
cricket::MediaSessionDescriptionFactory::AddAudioContentForOffer(cricket::MediaDescriptionOptions const&, cricket::MediaSessionOptions const&, cricket::ContentInfo const*, cricket::SessionDescription const*, std::vector<webrtc::RtpExtension, std::allocator<webrtc::RtpExtension> > const&, std::vector<cricket::AudioCodec, std::allocator<cricket::AudioCodec> > const&, std::vector<cricket::StreamParams, std::allocator<cricket::StreamParams> >*, cricket::SessionDescription*, cricket::IceCredentialsIterator*) const [inlined] bool cricket::CreateMediaContentOffer<cricket::AudioCodec>(cricket::MediaDescriptionOptions const&, cricket::MediaSessionOptions const&, std::vector<cricket::AudioCodec, std::allocator<cricket::AudioCodec> > const&, cricket::SecurePolicy const&, std::vector<cricket::CryptoParams, std::allocator<cricket::CryptoParams> > const*, std::vector<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > const&, std::vector<webrtc::RtpExtension, std::allocator<webrtc::RtpExtension> > const&, rtc::UniqueRandomIdGenerator*, std::vector<cricket::StreamParams, std::allocator<cricket::StreamParams> >*, cricket::MediaContentDescriptionImpl<cricket::AudioCodec>*) at media_session.cc:764
cricket::MediaSessionDescriptionFactory::AddAudioContentForOffer(cricket::MediaDescriptionOptions const&, cricket::MediaSessionOptions const&, cricket::ContentInfo const*, cricket::SessionDescription const*, std::vector<webrtc::RtpExtension, std::allocator<webrtc::RtpExtension> > const&, std::vector<cricket::AudioCodec, std::allocator<cricket::AudioCodec> > const&, std::vector<cricket::StreamParams, std::allocator<cricket::StreamParams> >*, cricket::SessionDescription*, cricket::IceCredentialsIterator*) const at media_session.cc:2047
cricket::MediaSessionDescriptionFactory::CreateOffer(cricket::MediaSessionOptions const&, cricket::SessionDescription const*) const at media_session.cc:1471
webrtc::WebRtcSessionDescriptionFactory::InternalCreateOffer(webrtc::CreateSessionDescriptionRequest) at webrtc_session_description_factory.cc:339
webrtc::WebRtcSessionDescriptionFactory::CreateOffer(webrtc::CreateSessionDescriptionObserver*, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions const&, cricket::MediaSessionOptions const&) at webrtc_session_description_factory.cc:240
webrtc::PeerConnection::CreateOffer(webrtc::CreateSessionDescriptionObserver*, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions const&) at peer_connection.cc:2008
void webrtc::ReturnType<void>::Invoke<webrtc::PeerConnectionInterface, void (webrtc::PeerConnectionInterface::*)(webrtc::CreateSessionDescriptionObserver*, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions const&), webrtc::CreateSessionDescriptionObserver*, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions>(webrtc::PeerConnectionInterface*, void (webrtc::PeerConnectionInterface::*)(webrtc::CreateSessionDescriptionObserver*, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions const&), webrtc::CreateSessionDescriptionObserver*, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions) at proxy.h:131
webrtc::internal::SynchronousMethodCall::OnMessage(rtc::Message*) at proxy.cc:32
```
> generate code
```c++
void StreamParams::GenerateSsrcs(int num_layers,
                                 bool generate_fid,
                                 bool generate_fec_fr,
                                 rtc::UniqueRandomIdGenerator* ssrc_generator) {
  RTC_DCHECK_GE(num_layers, 0);
  RTC_DCHECK(ssrc_generator);
  std::vector<uint32_t> primary_ssrcs;
  for (int i = 0; i < num_layers; ++i) {
    uint32_t ssrc = ssrc_generator->GenerateId();
    primary_ssrcs.push_back(ssrc);
    add_ssrc(ssrc);
  }
...
```

### collect audio codec
```c++
webrtc::AudioEncoderOpusImpl::AppendSupportedEncoders(std::vector<webrtc::AudioCodecSpec, std::allocator<webrtc::AudioCodecSpec> >*) at audio_encoder_opus.cc:280
webrtc::audio_encoder_factory_template_impl::AudioEncoderFactoryT<webrtc::AudioEncoderOpus, webrtc::AudioEncoderIsacFloat, webrtc::AudioEncoderG722, webrtc::AudioEncoderIlbc, webrtc::AudioEncoderG711, webrtc::(anonymous namespace)::NotAdvertised<webrtc::AudioEncoderL16> >::GetSupportedEncoders() [inlined] webrtc::audio_encoder_factory_template_impl::Helper<webrtc::AudioEncoderOpus, webrtc::AudioEncoderIsacFloat, webrtc::AudioEncoderG722, webrtc::AudioEncoderIlbc, webrtc::AudioEncoderG711, webrtc::(anonymous namespace)::NotAdvertised<webrtc::AudioEncoderL16> >::AppendSupportedEncoders(std::vector<webrtc::AudioCodecSpec, std::allocator<webrtc::AudioCodecSpec> >*) at audio_encoder_factory_template.h:49
webrtc::audio_encoder_factory_template_impl::AudioEncoderFactoryT<webrtc::AudioEncoderOpus, webrtc::AudioEncoderIsacFloat, webrtc::AudioEncoderG722, webrtc::AudioEncoderIlbc, webrtc::AudioEncoderG711, webrtc::(anonymous namespace)::NotAdvertised<webrtc::AudioEncoderL16> >::GetSupportedEncoders() at audio_encoder_factory_template.h:82
cricket::WebRtcVoiceEngine::Init() at webrtc_voice_engine.cc:228
cricket::CompositeMediaEngine::Init() at media_engine.cc:155
rtc::FunctorMessageHandler<bool, cricket::ChannelManager::Init()::$_3>::OnMessage(rtc::Message*) [inlined] cricket::ChannelManager::Init()::$_3::operator()() const at channel_manager.cc:135
```
> default audio encoder factory
```c++
rtc::scoped_refptr<AudioEncoderFactory> CreateBuiltinAudioEncoderFactory() {
  return CreateAudioEncoderFactory<

#if WEBRTC_USE_BUILTIN_OPUS
      AudioEncoderOpus,
#endif

      AudioEncoderIsac, AudioEncoderG722,

#if WEBRTC_USE_BUILTIN_ILBC
      AudioEncoderIlbc,
#endif

      AudioEncoderG711, NotAdvertised<AudioEncoderL16>>();
}
```

### collect video codec
```c++
webrtc::(anonymous namespace)::CreateH264Format(webrtc::H264::Profile, webrtc::H264::Level, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) at h264.cc:52
webrtc::SupportedH264Codecs() at h264.cc:82
webrtc::InternalEncoderFactory::GetSupportedFormats() const at internal_encoder_factory.cc:32
cricket::(anonymous namespace)::AssignPayloadTypesAndDefaultCodecs(webrtc::VideoEncoderFactory const*) at webrtc_video_engine.cc:148
cricket::ChannelManager::GetSupportedVideoCodecs(std::vector<cricket::VideoCodec, std::allocator<cricket::VideoCodec> >*) const at channel_manager.cc:97
cricket::MediaSessionDescriptionFactory::MediaSessionDescriptionFactory(cricket::ChannelManager*, cricket::TransportDescriptionFactory const*, rtc::UniqueRandomIdGenerator*) at media_session.cc:1365
webrtc::WebRtcSessionDescriptionFactory::WebRtcSessionDescriptionFactory(rtc::Thread*, cricket::ChannelManager*, webrtc::PeerConnectionInternal*, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::unique_ptr<rtc::RTCCertificateGeneratorInterface, std::default_delete<rtc::RTCCertificateGeneratorInterface> >, rtc::scoped_refptr<rtc::RTCCertificate> const&, rtc::UniqueRandomIdGenerator*) at webrtc_session_description_factory.cc:133
webrtc::PeerConnection::Initialize(webrtc::PeerConnectionInterface::RTCConfiguration const&, webrtc::PeerConnectionDependencies) at peer_connection.cc:1176
webrtc::PeerConnectionFactory::CreatePeerConnection(webrtc::PeerConnectionInterface::RTCConfiguration const&, webrtc::PeerConnectionDependencies) at peer_connection_factory.cc:325
void webrtc::ReturnType<rtc::scoped_refptr<webrtc::PeerConnectionInterface> >::Invoke<webrtc::PeerConnectionFactoryInterface, rtc::scoped_refptr<webrtc::PeerConnectionInterface> (webrtc::PeerConnectionFactoryInterface::*)(webrtc::PeerConnectionInterface::RTCConfiguration const&, webrtc::PeerConnectionDependencies), webrtc::PeerConnectionInterface::RTCConfiguration, webrtc::PeerConnectionDependencies>(webrtc::PeerConnectionFactoryInterface*, rtc::scoped_refptr<webrtc::PeerConnectionInterface> (webrtc::PeerConnectionFactoryInterface::*)(webrtc::PeerConnectionInterface::RTCConfiguration const&, webrtc::PeerConnectionDependencies), webrtc::PeerConnectionInterface::RTCConfiguration, webrtc::PeerConnectionDependencies) at proxy.h:85
webrtc::MethodCall2<webrtc::PeerConnectionFactoryInterface, rtc::scoped_refptr<webrtc::PeerConnectionInterface>, webrtc::PeerConnectionInterface::RTCConfiguration const&, webrtc::PeerConnectionDependencies>::OnMessage(rtc::Message*) at proxy.h:252
webrtc::internal::SynchronousMethodCall::OnMessage(rtc::Message*) at proxy.cc:32
```

> add default rtp feedback
```c++
void AddDefaultFeedbackParams(VideoCodec* codec) {
  // Don't add any feedback params for RED and ULPFEC.
  if (codec->name == kRedCodecName || codec->name == kUlpfecCodecName)
    return;
  codec->AddFeedbackParam(FeedbackParam(kRtcpFbParamRemb, kParamValueEmpty));
  codec->AddFeedbackParam(
      FeedbackParam(kRtcpFbParamTransportCc, kParamValueEmpty));
  // Don't add any more feedback params for FLEXFEC.
  if (codec->name == kFlexfecCodecName || codec->name == kRSfecCodecName)
    return;
  codec->AddFeedbackParam(FeedbackParam(kRtcpFbParamCcm, kRtcpFbCcmParamFir));
  codec->AddFeedbackParam(FeedbackParam(kRtcpFbParamNack, kParamValueEmpty));
  codec->AddFeedbackParam(FeedbackParam(kRtcpFbParamNack, kRtcpFbNackParamPli));
}
```

> gather all extension
```c++
void ChannelManager::GetSupportedVideoRtpHeaderExtensions(
    RtpHeaderExtensions* ext) const {
  if (!media_engine_) {
    return;
  }
  *ext = media_engine_->video().GetCapabilities().header_extensions;
}

RtpCapabilities WebRtcVideoEngine::GetCapabilities() const {
  RtpCapabilities capabilities;
  int id = 1;
  capabilities.header_extensions.push_back(
      webrtc::RtpExtension(webrtc::RtpExtension::kTimestampOffsetUri, id++));
  capabilities.header_extensions.push_back(
      webrtc::RtpExtension(webrtc::RtpExtension::kAbsSendTimeUri, id++));
  capabilities.header_extensions.push_back(
      webrtc::RtpExtension(webrtc::RtpExtension::kVideoRotationUri, id++));
  capabilities.header_extensions.push_back(webrtc::RtpExtension(
      webrtc::RtpExtension::kTransportSequenceNumberUri, id++));
  capabilities.header_extensions.push_back(
      webrtc::RtpExtension(webrtc::RtpExtension::kPlayoutDelayUri, id++));
  capabilities.header_extensions.push_back(
      webrtc::RtpExtension(webrtc::RtpExtension::kVideoContentTypeUri, id++));
  capabilities.header_extensions.push_back(
      webrtc::RtpExtension(webrtc::RtpExtension::kVideoTimingUri, id++));
  capabilities.header_extensions.push_back(
      webrtc::RtpExtension(webrtc::RtpExtension::kFrameMarkingUri, id++));
  capabilities.header_extensions.push_back(
      webrtc::RtpExtension(webrtc::RtpExtension::kColorSpaceUri, id++));
  if (webrtc::field_trial::IsEnabled("WebRTC-GenericDescriptorAdvertised")) {
    capabilities.header_extensions.push_back(webrtc::RtpExtension(
        webrtc::RtpExtension::kGenericFrameDescriptorUri00, id++));
    capabilities.header_extensions.push_back(webrtc::RtpExtension(
        webrtc::RtpExtension::kGenericFrameDescriptorUri01, id++));
  }

  return capabilities;
}
```

> create rtx codec pt
```c++
// Add associated RTX codec for non-FEC codecs.
if (!absl::EqualsIgnoreCase(codec.name, kUlpfecCodecName) &&
    !absl::EqualsIgnoreCase(codec.name, kRSfecCodecName) &&
    !absl::EqualsIgnoreCase(codec.name, kFlexfecCodecName)) {
  output_codecs.push_back(
      VideoCodec::CreateRtxCodec(payload_type, codec.id));

  // Increment payload type.
  ++payload_type;
  if (payload_type > kLastDynamicPayloadType) {
    RTC_LOG(LS_ERROR) << "Out of dynamic payload types, skipping the rest.";
    break;
  }
}
```

> build sdp rtpmap and fmtp line
```c++
for (const cricket::AudioCodec& codec : media_desc->as_audio()->codecs()) {
  RTC_DCHECK(!codec.name.empty());
  // RFC 4566
  // a=rtpmap:<payload type> <encoding name>/<clock rate>
  // [/<encodingparameters>]
  InitAttrLine(kAttributeRtpmap, &os);
  os << kSdpDelimiterColon << codec.id << " ";
  os << codec.name << "/" << codec.clockrate;
  if (codec.channels != 1) {
    os << "/" << codec.channels;
  }
  AddLine(os.str(), message);
  AddRtcpFbLines(codec, message);
  AddFmtpLine(codec, message);
  ...
}
```

### mid extension
```c++
class RtpMid : public BaseRtpStringExtension {
 public:
  static constexpr RTPExtensionType kId = kRtpExtensionMid;
  static constexpr const char kUri[] = "urn:ietf:params:rtp-hdrext:sdes:mid";
};
```
> sdp line:
> a=group:BUNDLE audio video
> a=mid:audio
> a=mid:video

### msid,mslabel,label
> sdp line:
> a=ssrc:1124722508 msid:353611640938578776 353611640938578776
> a=ssrc:1124722508 mslabel:353611640938578776
> a=ssrc:1124722508 label:353611640938578776

> rfc: draft-alvestrand-mmusic-msid-01
> The value of the msid corresponds to the "id" attribute of a
   MediaStream. (note: as of Jan 11, 2012, this is called "label".  The
   word "label" means many other things, so the same word should not be
   used.)

msid:
```c++
void StreamParams::set_stream_ids(const std::vector<std::string>& stream_ids) {
  stream_ids_ = stream_ids;
}
```
passed from AddTrack's 'stream_ids', used in PeerConnection::CreateSender()
```c++
RTCErrorOr<rtc::scoped_refptr<RtpSenderInterface>> AddTrack(
    rtc::scoped_refptr<MediaStreamTrackInterface> track,
    const std::vector<std::string>& stream_ids);
```

cname:
```c++
std::string GenerateRtcpCname() {
  std::string cname;
  if (!rtc::CreateRandomString(kRtcpCnameLength, &cname)) {
    RTC_LOG(LS_ERROR) << "Failed to generate CNAME.";
    RTC_NOTREACHED();
  }
  return cname;
}
```

### assign video payload type
```c++
cricket::(anonymous namespace)::AssignPayloadTypesAndDefaultCodecs(webrtc::VideoEncoderFactory const*) [inlined] cricket::(anonymous namespace)::AssignPayloadTypesAndDefaultCodecs(std::vector<webrtc::SdpVideoFormat, std::allocator<webrtc::SdpVideoFormat> >) at webrtc_video_engine.cc:118
cricket::(anonymous namespace)::AssignPayloadTypesAndDefaultCodecs(webrtc::VideoEncoderFactory const*) at webrtc_video_engine.cc:155
cricket::ChannelManager::GetSupportedVideoCodecs(std::vector<cricket::VideoCodec, std::allocator<cricket::VideoCodec> >*) const at channel_manager.cc:97
cricket::MediaSessionDescriptionFactory::MediaSessionDescriptionFactory(cricket::ChannelManager*, cricket::TransportDescriptionFactory const*, rtc::UniqueRandomIdGenerator*) at media_session.cc:1365
webrtc::WebRtcSessionDescriptionFactory::WebRtcSessionDescriptionFactory(rtc::Thread*, cricket::ChannelManager*, webrtc::PeerConnectionInternal*, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::unique_ptr<rtc::RTCCertificateGeneratorInterface, std::default_delete<rtc::RTCCertificateGeneratorInterface> >, rtc::scoped_refptr<rtc::RTCCertificate> const&, rtc::UniqueRandomIdGenerator*) at webrtc_session_description_factory.cc:133
webrtc::PeerConnection::Initialize(webrtc::PeerConnectionInterface::RTCConfiguration const&, webrtc::PeerConnectionDependencies) at peer_connection.cc:1176
webrtc::PeerConnectionFactory::CreatePeerConnection(webrtc::PeerConnectionInterface::RTCConfiguration const&, webrtc::PeerConnectionDependencies) at peer_connection_factory.cc:325
void webrtc::ReturnType<rtc::scoped_refptr<webrtc::PeerConnectionInterface> >::Invoke<webrtc::PeerConnectionFactoryInterface, rtc::scoped_refptr<webrtc::PeerConnectionInterface> (webrtc::PeerConnectionFactoryInterface::*)(webrtc::PeerConnectionInterface::RTCConfiguration const&, webrtc::PeerConnectionDependencies), webrtc::PeerConnectionInterface::RTCConfiguration, webrtc::PeerConnectionDependencies>(webrtc::PeerConnectionFactoryInterface*, rtc::scoped_refptr<webrtc::PeerConnectionInterface> (webrtc::PeerConnectionFactoryInterface::*)(webrtc::PeerConnectionInterface::RTCConfiguration const&, webrtc::PeerConnectionDependencies), webrtc::PeerConnectionInterface::RTCConfiguration, webrtc::PeerConnectionDependencies) at proxy.h:85
webrtc::MethodCall2<webrtc::PeerConnectionFactoryInterface, rtc::scoped_refptr<webrtc::PeerConnectionInterface>, webrtc::PeerConnectionInterface::RTCConfiguration const&, webrtc::PeerConnectionDependencies>::OnMessage(rtc::Message*) at proxy.h:252
```

### audio set send codec
```c++
cricket::WebRtcVoiceMediaChannel::SetSendCodecs(std::vector<cricket::AudioCodec, std::allocator<cricket::AudioCodec> > const&) at webrtc_voice_engine.cc:1672
cricket::WebRtcVoiceMediaChannel::SetSendParameters(cricket::AudioSendParameters const&) at webrtc_voice_engine.cc:1306
cricket::VoiceChannel::SetRemoteContent_w(cricket::MediaContentDescription const*, webrtc::SdpType, std::basic_string<char, std::char_traits<char>, std::allocator<char> >*) at channel.cc:941
rtc::FunctorMessageHandler<bool, rtc::MethodFunctor<cricket::BaseChannel, bool (cricket::BaseChannel::*)(cricket::MediaContentDescription const*, webrtc::SdpType, std::basic_string<char, std::char_traits<char>, std::allocator<char> >*), bool, cricket::MediaContentDescription const*, webrtc::SdpType, std::basic_string<char, std::char_traits<char>, std::allocator<char> >*> const&>::OnMessage(rtc::Message*) at message_handler.h:42
```

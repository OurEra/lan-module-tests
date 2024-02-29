### receive rtp packet (from transport)
```c++
webrtc::RtpPacket::Parse(rtc::CopyOnWriteBuffer) at rtp_packet.cc:90
webrtc::RtpTransport::DemuxPacket(rtc::CopyOnWriteBuffer, long) at rtp_transport.cc:190
webrtc::SrtpTransport::OnRtpPacketReceived(rtc::CopyOnWriteBuffer, long) at srtp_transport.cc:230
webrtc::RtpTransport::OnReadPacket(rtc::PacketTransportInternal*, char const*, unsigned long, long const&, int) at rtp_transport.cc:273
cricket::P2PTransportChannel::OnReadPacket(cricket::Connection*, char const*, unsigned long, long) at p2p_transport_channel.cc:2396
cricket::Connection::OnReadPacket(char const*, unsigned long, long) at port.cc:1267
cricket::UDPPort::HandleIncomingPacket(rtc::AsyncPacketSocket*, char const*, unsigned long, rtc::SocketAddress const&, long) at stun_port.cc:342
rtc::AsyncUDPSocket::OnReadEvent(rtc::AsyncSocket*) at async_udp_socket.cc:132
rtc::SocketDispatcher::OnEvent(unsigned int, int) at physical_socket_server.cc:786
rtc::ProcessEvents(rtc::Dispatcher*, bool, bool, bool) at physical_socket_server.cc:1375
rtc::PhysicalSocketServer::WaitEpoll(int) at physical_socket_server.cc:1616
```

### receive rtcp packet (from transport)
```c++
cricket::BaseChannel::OnRtcpPacketReceived(rtc::CopyOnWriteBuffer*, long) at channel.cc:500
webrtc::SrtpTransport::OnRtcpPacketReceived(rtc::CopyOnWriteBuffer, long) at srtp_transport.cc:251
webrtc::RtpTransport::OnReadPacket(rtc::PacketTransportInternal*, char const*, unsigned long, long const&, int) at rtp_transport.cc:271
cricket::P2PTransportChannel::OnReadPacket(cricket::Connection*, char const*, unsigned long, long) at p2p_transport_channel.cc:2396
cricket::Connection::OnReadPacket(char const*, unsigned long, long) at port.cc:1267
cricket::UDPPort::HandleIncomingPacket(rtc::AsyncPacketSocket*, char const*, unsigned long, rtc::SocketAddress const&, long) at stun_port.cc:342
rtc::AsyncUDPSocket::OnReadEvent(rtc::AsyncSocket*) at async_udp_socket.cc:132
rtc::SocketDispatcher::OnEvent(unsigned int, int) at physical_socket_server.cc:786
rtc::ProcessEvents(rtc::Dispatcher*, bool, bool, bool) at physical_socket_server.cc:1375
rtc::PhysicalSocketServer::WaitEpoll(int) at physical_socket_server.cc:1616
```

### build ReceiverReport
```c++
webrtc::RTCPSender::BuildRR(webrtc::RTCPSender::RtcpContext const&) at rtcp_sender.cc:483
webrtc::RTCPSender::SendCompoundRTCP(webrtc::RTCPSender::FeedbackState const&, std::__1::set<webrtc::RTCPPacketType, std::__1::less<webrtc::RTCPPacketType>, std::__1::allocator<webrtc::RTCPPacketType> > const&, int, unsigned short const*) at rtcp_sender.cc:742
webrtc::RTCPSender::SendRTCP(webrtc::RTCPSender::FeedbackState const&, webrtc::RTCPPacketType, int, unsigned short const*) at rtcp_sender.cc:680
webrtc::ModuleRtpRtcpImpl::Process() at rtp_rtcp_impl.cc:219
```
> block class structure
```c++
class ReportBlock {
 public:
  static const size_t kLength = 24;

  ReportBlock();
  ~ReportBlock() {}

  bool Parse(const uint8_t* buffer, size_t length);

  // Fills buffer with the ReportBlock.
  // Consumes ReportBlock::kLength bytes.
  void Create(uint8_t* buffer) const;

  void SetMediaSsrc(uint32_t ssrc) { source_ssrc_ = ssrc; }
  void SetFractionLost(uint8_t fraction_lost) {
    fraction_lost_ = fraction_lost;
  }
  bool SetCumulativeLost(int32_t cumulative_lost);
  ...
}
```
> calculate fraction lost
```c++
  int32_t missing = 0;
  if (exp_since_last > rec_since_last) {
    missing = (exp_since_last - rec_since_last);
  }
  uint8_t local_fraction_lost = 0;
  if (exp_since_last) {
    // Scale 0 to 255, where 255 is 100% loss.
    local_fraction_lost = static_cast<uint8_t>(255 * missing / exp_since_last);
  }
  stats.fraction_lost = local_fraction_lost;
```

### calculate RTT
```c++
if (send_time_ntp != 0) {
  uint32_t delay_ntp = report_block.delay_since_last_sr();
  // Local NTP time.
  uint32_t receive_time_ntp =
      CompactNtp(TimeMicrosToNtp(clock_->TimeInMicroseconds()));

  // RTT in 1/(2^16) seconds.
  uint32_t rtt_ntp = receive_time_ntp - delay_ntp - send_time_ntp;
  // Convert to 1/1000 seconds (milliseconds).
  rtt_ms = CompactNtpRttToMs(rtt_ntp);
  if (rtt_ms > report_block_info->max_rtt_ms)
    report_block_info->max_rtt_ms = rtt_ms;

  if (report_block_info->num_rtts == 0 ||
      rtt_ms < report_block_info->min_rtt_ms)
    report_block_info->min_rtt_ms = rtt_ms;

  report_block_info->last_rtt_ms = rtt_ms;
  report_block_info->sum_rtt_ms += rtt_ms;
  ++report_block_info->num_rtts;

  packet_information->rtt_ms = rtt_ms;
}
```

### build SenderReport
```c++
webrtc::RTCPSender::BuildSR(webrtc::RTCPSender::RtcpContext const&) at rtcp_sender.cc:458
webrtc::RTCPSender::SendCompoundRTCP(webrtc::RTCPSender::FeedbackState const&, std::__1::set<webrtc::RTCPPacketType, std::__1::less<webrtc::RTCPPacketType>, std::__1::allocator<webrtc::RTCPPacketType> > const&, int, unsigned short const*) at rtcp_sender.cc:746
webrtc::RTCPSender::SendRTCP(webrtc::RTCPSender::FeedbackState const&, webrtc::RTCPPacketType, int, unsigned short const*) at rtcp_sender.cc:684
webrtc::ModuleRtpRtcpImpl::Process() at rtp_rtcp_impl.cc:219
webrtc::ProcessThreadImpl::Process() at process_thread_impl.cc:192
```

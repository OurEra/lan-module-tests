### create play
```c++
srs`SrsRtcPlayStream::SrsRtcPlayStream(this=0x000055803389da70, s=0x00005580338f0a30, cid=0x00005580338994e8) at srs_app_rtc_conn.cpp:378
srs`SrsRtcConnection::create_player(this=0x00005580338f0a30, req=0x00005580339a9f50, sub_relations=size=2) at srs_app_rtc_conn.cpp:3351
srs`SrsRtcConnection::add_player(this=0x00005580338f0a30, req=0x00005580339a9f50, remote_sdp=0x00005580339aa240, local_sdp=0x00005580339aa4f0) at srs_app_rtc_conn.cpp:1822
srs`SrsRtcServer::do_create_session(this=0x00005580335e2a60, session=0x00005580338f0a30, req=0x00005580339a9f50, remote_sdp=0x00005580339aa240, local_sdp=0x00005580339aa4f0, mock_eip="0\x9e\x9a3\x80U"..., publish=false, dtls=true, srtp=true) at srs_app_rtc_server.cpp:444
srs`SrsRtcServer::create_session(this=0x00005580335e2a60, req=0x00005580339a9f50, remote_sdp=0x00005580339aa240, local_sdp=0x00005580339aa4f0, mock_eip="0\x9e\x9a3\x80U"..., publish=false, dtls=true, srtp=true, psession=0x00005580339a9d10) at srs_app_rtc_server.cpp:421
srs`SrsGoApiRtcPlay::do_serve_http(this=0x0000558033613960, w=0x00005580339aab00, r=0x00005580338d4a30, res=0x0000558033950c40) at srs_app_rtc_api.cpp:194
srs`SrsGoApiRtcPlay::serve_http(this=0x0000558033613960, w=0x00005580339aab00, r=0x00005580338d4a30) at srs_app_rtc_api.cpp:67
srs`SrsHttpServeMux::serve_http(this=0x00005580335e2860, w=0x00005580339aab00, r=0x00005580338d4a30) at srs_http_stack.cpp:732
srs`SrsHttpCorsMux::serve_http(this=0x0000558033992da0, w=0x00005580339aab00, r=0x00005580338d4a30) at srs_http_stack.cpp:880
srs`SrsHttpConn::process_request(this=0x0000558033791db0, w=0x00005580339aab00, r=0x00005580338d4a30, rid=1) at srs_app_http_conn.cpp:250
srs`SrsHttpConn::process_requests(this=0x0000558033791db0, preq=0x00005580339aabd8) at srs_app_http_conn.cpp:223
srs`SrsHttpConn::do_cycle(this=0x0000558033791db0) at srs_app_http_conn.cpp:177
srs`SrsHttpConn::cycle(this=0x0000558033791db0) at srs_app_http_conn.cpp:122
srs`SrsSTCoroutine::cycle(this=0x000055803378b870) at srs_app_st.cpp:226
srs`SrsSTCoroutine::pfn(arg=0x000055803378b870) at srs_app_st.cpp:241
```

### on rtp to consumer
```c++
`SrsRtcStream::on_rtp(this=0x00005580335e2f00, pkt=0x00005580338f87b0) at srs_app_rtc_source.cpp:518
`SrsRtcFromRtmpBridger::on_audio(this=0x00005580336f60e0, msg=0x00007f9c5662aab0) at srs_app_rtc_source.cpp:706
`SrsSource::on_audio_imp(this=0x00005580337219e0, msg=0x00007f9c5662aab0) at srs_app_source.cpp:2231
`SrsSource::on_audio(this=0x00005580337219e0, shared_audio=0x0000558033799f30) at srs_app_source.cpp:2186
`SrsRtmpConn::process_publish_message(this=0x00005580338a6680, source=0x00005580337219e0, msg=0x0000558033799f30) at srs_app_rtmp_conn.cpp:1043
`SrsRtmpConn::handle_publish_message(this=0x00005580338a6680, source=0x00005580337219e0, msg=0x0000558033799f30) at srs_app_rtmp_conn.cpp:1022
`SrsPublishRecvThread::consume(this=0x0000558033831900, msg=0x0000558033799f30) at srs_app_recv_thread.cpp:395
`SrsRecvThread::do_cycle(this=0x0000558033831908) at srs_app_recv_thread.cpp:150
`SrsRecvThread::cycle(this=0x0000558033831908) at srs_app_recv_thread.cpp:119
`SrsSTCoroutine::cycle(this=0x00005580337a9ba0) at srs_app_st.cpp:226
`SrsSTCoroutine::pfn(arg=0x00005580337a9ba0) at srs_app_st.cpp:241
```

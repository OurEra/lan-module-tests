### use aecdump

> unnpack aecdump
> 1. gn gen out/host-debug --args='is_component_build=false rtc_use_h264=true ffmpeg_branding="Chrome"'
> 2. ninja -C out/host-debug unpack_aecdump
> https://stackoverflow.com/questions/34338446/how-to-unzip-audio-aecdump

> filter data dump 
> 1. ./tools_webrtc/android/build_aar.py --build-dir ./out/xxx --arch arm64-v8a armeabi-v7a --extra-gn-args 'is_debug=true rtc_use_h264=true ffmpeg_branding="Chrome" apm_debug_dump=true'
> 2. +bool ApmDataDumper::recording_activated_ = true;
>    +char ApmDataDumper::output_dir_[] = "/sdcard/Qiniu/";

### use event log
1 接口调用保存 event log, 参考 https://webrtc.googlesource.com/src/+/refs/heads/main/examples/androidapp/src/org/appspot/apprtc/RtcEventLog.java
2 编译工具 event_log_visualizer
  > ninja -C out/host-debug/ event_log_visualizer

3 拿出 event log, 如 event_log_20221109_0555_11.log; 然后使用工具生成 python 脚本
  > event_log_visualizer event_log_20221109_0555_11.log > event_log.py

4 使用 python 运行,展示 event 图形:
  > python3 event_log.py

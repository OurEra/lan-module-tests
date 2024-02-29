platform select remote-android
platform connect unix-abstract-connect:///data/data/com.qiniu.droid.rtc.testing/debug.socket
process attach -p 6790
c
add-dsym /home/shiruiwei/qiniu/android_webrtc_m75/pili-webrtc/out/debug-build/arm64-v8a/lib.unstripped/libqndroid_rtc.so

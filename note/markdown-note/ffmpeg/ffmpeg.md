## 使用命令
* 1 打包h264/hevc
```
ffmpeg -i INPUT -c:v libx265 -an -x265-params crf=25 OUT.mp4
# With audio
ffmpeg -i INPUT -c:v libx265 -c:a copy -x265-params crf=25 OUT.mp4
ffmpeg -i "source.h264" -c:v copy -f mp4 "myOutputFile.mp4"
```
* 2 播放pcm
```
ffplay -ar 44100 -ac 1 -f s16le -i test.pcm

$ ffmpeg -formats | grep PCM
 DE alaw            PCM A-law
 DE f32be           PCM 32-bit floating-point big-endian
 DE f32le           PCM 32-bit floating-point little-endian
 DE f64be           PCM 64-bit floating-point big-endian
 DE f64le           PCM 64-bit floating-point little-endian
 DE mulaw           PCM mu-law
 DE s16be           PCM signed 16-bit big-endian
 DE s16le           PCM signed 16-bit little-endian
 DE s24be           PCM signed 24-bit big-endian
 DE s24le           PCM signed 24-bit little-endian
 DE s32be           PCM signed 32-bit big-endian
 DE s32le           PCM signed 32-bit little-endian
 DE s8              PCM signed 8-bit
 DE u16be           PCM unsigned 16-bit big-endian
 DE u16le           PCM unsigned 16-bit little-endian
 DE u24be           PCM unsigned 24-bit big-endian
 DE u24le           PCM unsigned 24-bit little-endian
 DE u32be           PCM unsigned 32-bit big-endian
 DE u32le           PCM unsigned 32-bit little-endian
 DE u8              PCM unsigned 8-bit
```
```
添加SEI
ffmpeg -i h264file.h264 -c copy -bsf:v h264_metadata=sei_user_data="05FBC6B9-5A80-40E5-A22A-AB4020267E26+hello" outsei.h264

https://docs.microsoft.com/en-us/openspecs/office_protocols/ms-h264pf/3d5fb9c1-efe8-4092-a60d-5321adce9c2e
```

````
查看profile/level
ffprobe -loglevel error -show_streams cam.mp4
````
````
修改profile/level
ffmpeg -i output_file.mp4 -c:v libx264 -profile:v main -level:v 4.0 -c:a copy output_level.mp4
````
````
丢弃audio stream
ffmpeg -i cam.mp4 -vcodec copy -an output_file.mp4
````
````
指定同步类型
ffplay -sync type       type=audio/video/ext
````

## 媒体播放
### 1 解析媒体格式
```C
  AVFormatContext
    .iformat [AVInputFormat *]
    .pb [AVIOContext *]
    .nb_streams [int]
    .streams [AVStream **]
    .video_codec [AVCodec *]
    .audio_codec [AVCodec *]
    .io_open [io_open_default]

AVStream
  .codecpar [AVCodecParameters *]

AVCodecParameters
  .codec_id [AVCodecID]
```

- 协议 (URLProtocol) 类型
  在 avformat_open_input() 中，对于一个资源首先需要确认它是一个媒体文件或者媒体在线地址，
具体在 init_input() 中通过 io_open() 即 io_open_default() 完成:

  **ffurl_open_whitelist() 确定协议类型**
  在 ffurl_alloc() 时，首先通过 url_find_protocol() 匹配 protocol_list.c 的能力, 返回一个 URLProtocol;
  然后 url_alloc_for_protocol() 中赋值给 URLContext.prot;

  在 ffurl_connect() 时，根据匹配到的 protocal, 比如文件的是 ff_file_protocol; 通过调用其实现的具体 url_open 来
  打开具体的文件，如果是文件类型，文件的 fd 则存放到了 URLContext.priv_data.fd

  **ffio_fdopen() 主要将 AVIOContext 和 URLContext 关联起来**
  ```C
  AVIOContext
    .opaque [AVIOInternal]
    .buffer [unsigned char*]

  AVIOInternal
    .h [URLContext]
  ```
  后续对于调用 AVIOContext.write_packet, seek 通过通用的 io_read_packet() 和 io_seek() 就关联到了具体的实现,
  如 URLContext.prot.url_read; 同时, 会分配好用于 IO 的 buffer, 默认为 **IO_BUFFER_SIZE 32768**


- 封装 (AVInputFormat) 类型
  在 init_input() 确定好协议类型后，然后会通过 av_probe_input_buffer2() 来确定媒体的封装类型:

  **avio_read() 读取头信息**
  为了确定封装类型，首先需要获取一些头信息，avio_read() 调用到具体 URLProtocol.url_read 实现

  **av_probe_input_format3() 匹配封装类型**
  通过遍历 demuxer_list.c 中的类型，并调用每个实现的 AVInputFormat.read_probe() 来和上一步 read 出来的数据匹配;
  匹配成功后对应的 AVInputFormat 则被确认并保存下来


- 音视频 (AVStream) 类型
  在 avformat_open_input() 中，当完成协议和封装信息的确认后，再会通过 AVInputFormat.read_header() 把封装格式中的音视频信息解析
  并存放到 AVFormatContext.streams 中的 AVStream;

  这里举例 mp4 的 mov_read_header() 就是通过 mov_read_default() 递归来解析 mp4 中的各个 box 信息，当解析到 'trak' 类型时就会通过
  mov_read_trak() 来创建 avformat_new_stream(); 然后对具体的 audio/video 'stsd' 类型解析时，把音视频的格式信息存放到 AVStream.codecpar


### 2 解码播放
- 确认解码器 (AVCodec)

av_read_frame() 读取 AVPacket, AVPacket.stream_index 关联到对应 AVStream

    frame #2: 0x000055c03370a0e0 ffplay`file_read(h=<unavailable>, buf=<unavailable>, size=<unavailable>) at file.c:118
    frame #3: 0x000055c033707fc8 ffplay`ffurl_read at avio.c:386
    frame #4: 0x000055c033707fb0 ffplay`ffurl_read(h=0x00007fdd18001b40, buf="", size=32768) at avio.c:421
    frame #5: 0x000055c0335bf065 ffplay`avio_read at aviobuf.c:538
    frame #6: 0x000055c0335bf058 ffplay`avio_read at aviobuf.c:587
    frame #7: 0x000055c0335bf009 ffplay`avio_read(s=0x00007fdd18009fc0, buf=<unavailable>, size=524) at aviobuf.c:680
    frame #8: 0x000055c0336d95c0 ffplay`append_packet_chunked(s=0x00007fdd18009fc0, pkt=0x00007fdd27bfece0, size=683) at utils.c:293
    frame #9: 0x000055c033631eef ffplay`mov_read_packet(s=0x00007fdd18000b80, pkt=<unavailable>) at mov.c:7765
    frame #10: 0x000055c0336de1ba ffplay`ff_read_packet(s=0x00007fdd18000b80, pkt=0x00007fdd27bfece0) at utils.c:862
    frame #11: 0x000055c0336dedb3 ffplay`read_frame_internal(s=0x00007fdd18000b80, pkt=0x00007fdd27bfece0) at utils.c:1589
    frame #12: 0x000055c0336dfb28 ffplay`av_read_frame(s=0x00007fdd18000b80, pkt=0x00007fdd27bfece0) at utils.c:1791
    frame #13: 0x000055c0333e69ed ffplay`read_thread(arg=0x000055c0366cf9c0) at ffplay.c:3008


## 媒体封装


## 相关
time_base
buffer
packet
frame

BFrame

sync

filter

flv ts

m3u8

rtmp

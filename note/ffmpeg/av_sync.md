## dts/pts
每个封装格式有自己组织存放 pts/dts 的方式:

### 1 MP4
ctts box:<br/>
CT<sub>(n)</sub> = DT<sub>(n)</sub> + CTTS<sub>(n)</sub><br/>
stts box:<br/>
DT<sub>(n+1)</sub>=DT<sub>(n)</sub>+STTS<sub>(n)</sub><br/>
elst box:<br/>
start offset

#### 1.1 首先从 stts 拿出 dts;
```C
  AVIndexEntry *e;
  ...
  e->timestamp = current_dts;
  ...
  current_dts += sc->stts_data[stts_index].duration;
```

#### 1.2 然后根据 ctts 拿到 pts;
```C
AVPacket *pkt
...
pkt->dts = sample->timestamp;
...
pkt->pts = pkt->dts + sc->dts_shift + sc->ctts_data[sc->ctts_index].duration;
```

#### 1.3 由于 elst 存在的原因, 会在 **mov_fix_index()** 中对 stream 的开始时间，timestamp 进行修改
```C
int64_t empty_edits_sum_duration = 0;
...
if (!found_non_empty_edit && edit_list_media_time == -1) {
  empty_edits_sum_duration += edit_list_duration;
  continue;
}
...
// Start time should be equal to zero or the duration of any empty edits.
st->start_time = empty_edits_sum_duration;
```
empty 类型 elst 修改 stream 的开始时间 (整体偏移)

```C
...
if (curr_cts < edit_list_media_time || curr_cts >= (edit_list_duration + edit_list_media_time)) {
  ...
  flags |= AVINDEX_DISCARD_FRAME;
} else {
  ...
  if (edit_list_start_encountered == 0) {
    edit_list_start_encountered = 1;
  }
}

...
if (add_index_entry(st, current->pos, edit_list_dts_counter, current->size,
    current->min_distance, flags) == -1) {
  av_log(mov->fc, AV_LOG_ERROR, "Cannot add index entry\n");
  break;
}

// Only start incrementing DTS in frame_duration amounts, when we encounter a frame in edit list.
if (edit_list_start_encountered > 0) {
  edit_list_dts_counter = edit_list_dts_counter + frame_duration;
}
```
非空类型 elst 控制开始使用的时间偏移

### 2 MPEG-TS
PES_packet.PTS_DTS_flags<br/>
> 
This is a 2-bit field. When the PTS_DTS_flags field is set to '10', the PTS fields shall be present in
the PES packet header. When the PTS_DTS_flags field is set to '11', both the PTS fields and DTS fields shall be present
in the PES packet header. When the PTS_DTS_flags field is set to '00' no PTS or DTS fields shall be present in the PES
packet header. The value '01' is forbidden

```C
PESContext *pes   = filter->u.pes_filter.opaque;
MpegTSContext *ts = pes->ts;
...
pes->pts = AV_NOPTS_VALUE;
pes->dts = AV_NOPTS_VALUE;
if ((flags & 0xc0) == 0x80) {
    pes->dts = pes->pts = ff_parse_pes_pts(r);
    r += 5;
} else if ((flags & 0xc0) == 0xc0) {
    pes->pts = ff_parse_pes_pts(r);
    r += 5;
    pes->dts = ff_parse_pes_pts(r);
    r += 5;
}
...
```

### 3 FLV
FLVTAG.Timestamp<br/>
AVCVIDEOPACKET.CompositionTime<br/>

```C
int64_t dts, pts = AV_NOPTS_VALUE;
...
dts  = avio_rb24(s->pb);
dts |= (unsigned)avio_r8(s->pb) << 24;
...
if (st->codecpar->codec_id == AV_CODEC_ID_H264 || st->codecpar->codec_id == AV_CODEC_ID_MPEG4) {
  // sign extension
  int32_t cts = (avio_rb24(s->pb) + 0xff800000) ^ 0xff800000;
  pts = av_sat_add64(dts, cts);
  ...
}
```

## sync
当播放时, 可以选择向音频，视频或者外部时钟进行对齐;
默认使用音频时钟对齐的情况下，音频渲染后 pts 将会直接更新:
```C
...
  if (!isnan(af->pts))
    is->audio_clock = af->pts + (double) af->frame->nb_samples / af->frame->sample_rate;
...
  set_clock_at(&is->audclk, is->audio_clock - (double)(2 * is->audio_hw_buf_size + is->audio_write_buf_size) / is->audio_tgt.bytes_per_sec,
                  is->audio_clock_serial, audio_callback_time / 1000000.0);

```

视频播放时，需要确认视频时钟和参考时钟的差异, 如果时间在时钟之前，则不更新视频渲染:
```C
...
last_duration = vp_duration(is, lastvp, vp);
delay = compute_target_delay(last_duration, is);

time= av_gettime_relative()/1000000.0;
if (time < is->frame_timer + delay) {
  *remaining_time = FFMIN(is->frame_timer + delay - time, *remaining_time);
  goto display;
}

...
```
如果时间在时钟之后，则更新视频渲染:

```C
...
if (!isnan(vp->pts))
   update_video_pts(is, vp->pts, vp->pos, vp->serial);
...
  frame_queue_next(&is->pictq);
```

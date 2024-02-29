#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// Article:
// https://www.jianshu.com/p/1cbe31baa711

#define LOG_COLOR_RED(format, ...) \
  printf("\033[0;31m" format "\033[0m\n", ##__VA_ARGS__)
#define LOG_COLOR_BLUE(format, ...) \
  printf("\033[0;34m" format "\033[0m\n", ##__VA_ARGS__)
#define LOG_COLOR_GREEN(format, ...) \
  printf("\033[0;32m" format "\033[0m\n", ##__VA_ARGS__)

#define FIXED_HEADER_SIZE 9
#define FIXED_TAG_SIZE 11
#define TAG_AUDIO_TYPE 8
#define AUDIO_DATA_FIXED_SIZE 2
#define TAG_VIDEO_TYPE 9
#define VIDEO_DATA_FIXED_SIZE 5

#define ADTS_HEADER_SIZE 7

template <typename T>
inline void ReadBigEndian(const char buf[], T* out) {
  *out = buf[0];
  for (size_t i = 1; i < sizeof(T); ++i) {
    *out <<= 8;
    // Must cast to uint8_t to avoid clobbering by sign extension.
    *out |= static_cast<uint8_t>(buf[i]);
  }
}

struct FlvHeader {
  uint8_t mark_f;
  uint8_t mark_l;
  uint8_t mark_v;
  uint8_t version;
  uint8_t reserv : 5, audio : 1, reserv2 : 1, video : 1;
  uint32_t data_offset;
};

struct AudioData {
  uint8_t format : 4, rate : 2, sound_size : 1, sound_type : 1;
  uint8_t packet_type;
  uint8_t* data_ref;
};

struct VideoData {
  uint8_t frame_type : 4, codec_id : 4;
  uint8_t avc_packet_type;
  uint32_t compostion_time : 24;
  uint8_t* data_ref;
};

struct FlvTag {
  uint8_t type;
  uint32_t data_size : 24;
  uint32_t timestamp : 24;
  uint8_t extend_ts;
  uint32_t steram_id : 24;
  union Data {
    AudioData audio;
    VideoData video;
  } data;
  uint8_t* raw_data;
};

struct AudioSpecificConfig {
  uint8_t object_type:5;
  uint8_t sam_freq_index:4;
  uint8_t channel_config:4;
};

void parseHeader(const uint8_t* buf, FlvHeader* header) {
  uint8_t index = 0;
  header->mark_f = buf[index++];
  header->mark_l = buf[index++];
  header->mark_v = buf[index++];
  header->version = buf[index++];
  header->audio = buf[index] >> 2 & 0x01;
  header->video = buf[index++] & 0x01;

  ReadBigEndian<uint32_t>((const char*)(buf + index), &header->data_offset);
}

void parseTag(const uint8_t* buf, size_t len, FlvTag* tag) {
  if (len != FIXED_TAG_SIZE) {
    LOG_COLOR_RED("invalide size for parseTag");
    return;
  }
  uint8_t index = 0;
  tag->type = buf[index++];
  tag->data_size = buf[index++] << 16 | buf[index++] << 8 | buf[index++];
  tag->timestamp = buf[index++] << 16 | buf[index++] << 8 | buf[index++];
  tag->extend_ts = buf[index++];
  tag->steram_id = buf[index++] << 16 | buf[index++] << 8 | buf[index++];
  tag->raw_data = (uint8_t*)malloc(tag->data_size);
}

void parseAudioData(FlvTag* tag) {
  uint8_t index = 0;
  tag->data.audio.format = tag->raw_data[index] >> 4 & 0x0F;
  tag->data.audio.rate = tag->raw_data[index] >> 2 & 0X03;
  tag->data.audio.sound_size = tag->raw_data[index] >> 1 & 0x01;
  tag->data.audio.sound_type = tag->raw_data[index++] & 0x01;
  tag->data.audio.packet_type = tag->raw_data[index++];
  tag->data.audio.data_ref = tag->raw_data + index;
}

// audioObjectType; 5
// samplingFrequencyIndex; 4 bslbf
// channelConfiguration; 4 bsl
void parseAudioConfig(AudioData* data, AudioSpecificConfig* config) {
  config->object_type = (data->data_ref[0] >> 3) & 0x1F;
  config->sam_freq_index = (data->data_ref[0] & 0x07) << 1 | (data->data_ref[1] >> 7) & 0x01;
  config->channel_config = data->data_ref[1] >> 3 & 0x0F;
}

void parseVideoData(FlvTag* tag) {
  uint8_t index = 0;
  tag->data.video.frame_type = tag->raw_data[index] >> 4 & 0x0F;
  tag->data.video.codec_id = tag->raw_data[index++] & 0x0F;
  tag->data.video.avc_packet_type = tag->raw_data[index++];
  if (tag->data.video.avc_packet_type == 1) {
    tag->data.video.compostion_time = tag->raw_data[index++] << 16 |
                                      tag->raw_data[index++] << 8 |
                                      tag->raw_data[index++];
  } else {
    tag->data.video.compostion_time = 0;
    index += 3;
  }
  tag->data.video.data_ref = tag->raw_data + index;
}

// adts_fixed_header()
// {
//  syncword; 12 bslbf
//  ID; 1 bslbf
//  layer; 2 uimsbf
//  protection_absent; 1 bslbf
//
//  profile_ObjectType; 2 uimsbf
//  sampling_frequency_index; 4 uimsbf
//  private_bit; 1 bslbf
//  channel_configuration; 3 uimsbf
//  original_copy; 1 bslbf
//  home; 1 bslbf
// }
//
// adts_variable_header()
// {
//  copyright_identification_bit; 1 bslbf
//  copyright_identification_start; 1 bslbf
//  aac_frame_length; 13 bslbf
//  adts_buffer_fullness; 11 bslbf
//  number_of_raw_data_blocks_in_frame; 2 uimsbf
// }

void fillAdtsHeader(uint8_t* header, AudioSpecificConfig* config, uint32_t frame_len) {
  // byte 1
  {
  // sync
  header[0] = 0xFF;
  }
  // byte 2
  {
  header[1] = 0x0F << 4;
  // id
  header[1] |= (0x00 << 3); // MPEG-4
  // layer
  header[1] |= (0x00 << 2);
  header[1] |= (0x00 << 1);
  // protect absent
  header[1] |= 0x1;
  }
  // byte 3
  {
  // profile object type
  // If ID is equal to ‘0’ this element denotes the MPEG-4 Audio Object Type (profile_ObjectType+1) ...
  header[2] |= ((config->object_type - 1) & 0x03) << 6;
  // sam freq
  header[2] |= (config->sam_freq_index & 0x0F) << 2;
  // private bit
  header[2] |= 0x00 << 1;
  // channel config
  header[2] |= (config->channel_config >> 2) & 0x01;
  }
  // byte 4
  uint32_t length = ADTS_HEADER_SIZE + frame_len;
  {
  // channel config
  header[3] |= (config->channel_config & 0x03) << 6;
  // original
  header[3] |= 0x00 << 5;
  // home
  header[3] |= 0x00 << 4;
  // copyright
  header[3] |= 0x00 << 3;
  // copyright
  header[3] |= 0x00 << 2;
  //aac framelength
  header[3] |= (length >> 11) & 0x03;
  }
  // byte 5
  {
  //aac framelength
  header[4] |= (length >> 3) & 0xFF;
  }
  // byte 6
  {
  //aac framelength
  header[5] |= (length & 0x07) << 5;
  // buffer fullness
  header[5] |= 0x1f;
  }
  // byte 7
  {
  // buffer fullness
  header[6] |= 0x3F << 2;
  // raw blocks
  header[6] |= 0x00 << 1;
  header[6] |= 0x00;
  }
}

int main() {
  FILE* file = fopen("./test.flv", "r");
  FILE* audio_file = fopen("audio.aac", "w+");
  FILE* video_file = fopen("video.h264", "w+");
  uint8_t read_buffer[64] = {0};
  size_t read = 0;

  int tag_count = 0;

  if (!audio_file || !video_file || !file) {
    LOG_COLOR_RED("open file error: %s", strerror(errno));
    goto FINISH;
  }

  // 1. read header
  read = fread(read_buffer, 1, FIXED_HEADER_SIZE, file);

  FlvHeader header;
  memset(&header, 0, sizeof(FlvHeader));
  parseHeader(read_buffer, &header);
  LOG_COLOR_BLUE("=== %c%c%c audio %u video %u off %d", header.mark_f,
                 header.mark_l, header.mark_v, header.audio, header.video,
                 header.data_offset);

  AudioSpecificConfig audio_config;
  memset(&audio_config, 0, sizeof(AudioSpecificConfig));

  while (1) {
    // 2. read tag
    uint32_t pre_tag_size = 0;
    uint8_t adts_header[ADTS_HEADER_SIZE] = {0};
    read = fread(read_buffer, 1, sizeof(pre_tag_size), file);
    if (read < sizeof(pre_tag_size)) {
      LOG_COLOR_RED("parse file over");
      break;
    }
    tag_count++;
    ReadBigEndian<uint32_t>((const char*)read_buffer, &pre_tag_size);

    read = fread(read_buffer, 1, FIXED_TAG_SIZE, file);
    FlvTag tag;
    memset(&tag, 0, sizeof(FlvTag));
    parseTag(read_buffer, FIXED_TAG_SIZE, &tag);

    fread(tag.raw_data, 1, tag.data_size, file);
    if (tag.type == TAG_AUDIO_TYPE) {
      parseAudioData(&tag);
      LOG_COLOR_BLUE("=== count %d tag type %d data size %d", tag_count, tag.type, tag.data_size);
      LOG_COLOR_BLUE("    === audio format %d rate %d size %d type %d packet %d",
                     tag.data.audio.format, tag.data.audio.rate,
                     tag.data.audio.sound_size, tag.data.audio.sound_type,
                     tag.data.audio.packet_type);
      if (tag.data.audio.packet_type == 0) {
        parseAudioConfig(&tag.data.audio, &audio_config);
      } else {
        fillAdtsHeader(adts_header, &audio_config, tag.data_size - AUDIO_DATA_FIXED_SIZE);
        fwrite(adts_header, 1, ADTS_HEADER_SIZE, audio_file);
        fwrite(tag.data.audio.data_ref, 1, tag.data_size - AUDIO_DATA_FIXED_SIZE, audio_file);
      }
    } else if (tag.type == TAG_VIDEO_TYPE) {
      //parseVideoData(&tag);
      //LOG_COLOR_BLUE("    === video frame %d codec %d packet %d",
      //               tag->data.video.frame_type, tag->data.video.codec_id,
      //               tag->data.video.avc_packet_type);
      //fwrite(tag.data.video.data_ref, 1, tag.data_size - VIDEO_DATA_FIXED_SIZE, video_file);
    }
    free(tag.raw_data);
  }

FINISH:
  if (file) {
    fclose(file);
  }
  if (audio_file) {
    fclose(audio_file);
  }
  if (video_file) {
    fclose(video_file);
  }
  return 0;
}

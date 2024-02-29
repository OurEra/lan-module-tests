#include <stdio.h>
#include <string.h>
#include <time.h>

#include <algorithm>
#include <string>
#include <vector>

#include "bits_operation.h"

// Tools: https://mlynoteka.mlyn.org/mp4parser/
// Article: https://zhuanlan.zhihu.com/p/355803589
//          https://www.cnblogs.com/ailumiyana/p/12907363.html

// clang-format off
// 14496-12
// Table 1 — Box types, structure, and cross-reference
// 8.5.2 Sample Description Box

// 14496-1
// 7.2.6.5 ES_Descriptor

// aligned(8) class Box (unsigned int(32) boxtype,
//
// optional unsigned int(8)[16] extended_type) {
// unsigned int(32) size;
// unsigned int(32) type = boxtype;
// if (size==1) {
//   unsigned int(64) largesize;
// } else if (size==0) {
// // box extends to end of file
// }
//
// if (boxtype==‘uuid’) {
//   unsigned int(8)[16] usertype = extended_type;
// }
//
// }

// clang-format on
#define MAKEBE32(a, b, c, d) ((a << 24) | (b << 16) | (c << 8) | (d))
#define BOX_FIX_SIZE 8

class IsoBox {
 public:
  enum ParseState { kParsing, kParsed };
  IsoBox(uint8_t* pkt, int32_t length) {
    // parse box size and type
    assert(length >= 8);
    bsize_ = MAKEBE32(pkt[0], pkt[1], pkt[2], pkt[3]);
    type_ = MAKEBE32(pkt[4], pkt[5], pkt[6], pkt[7]);
    parsed_ = BOX_FIX_SIZE;
    state_ = kParsing;
    // init field
    data_ = NULL;
    dsize_ = 0;
  }

  int32_t MoreData(uint8_t* pkt, int32_t length) {
    int32_t need = bsize_ - parsed_ - dsize_;
    int32_t used = 0;
    if (need > 0) {
      used = std::min(need, length);
      uint8_t* naddr = (uint8_t*)malloc(used + dsize_);
      if (data_ != NULL) {
        memcpy(naddr, data_, dsize_);
      }
      memcpy(naddr + dsize_, pkt, used);
      if (data_ != NULL) {
        free(data_);
      }
      data_ = naddr;
      dsize_ += used;
    }
    return used;
  }

  ~IsoBox() {
    for (IsoBox* sub : sub_boxs) {
      delete sub;
    }
    if (data_) {
      free(data_);
    }
  }

  size_t ParsedSize() { return parsed_; }

  bool IsParsed() { return state_ == kParsed; }

  bool IsAllArrive() { return (parsed_ + dsize_) == bsize_; }

  virtual void Parse() {}
  virtual std::string toString() { return std::string(); }

  void updateParse(int32_t parsed) {
    if (IsAllArrive() && parsed == dsize_) {
      state_ = kParsed;
    }
    parsed_ += parsed;
    int32_t left = dsize_ - parsed;
    if (left) {
      uint8_t* naddr = (uint8_t*)malloc(left);
      memcpy(naddr, data_ + parsed, left);
      free(data_);
      data_ = naddr;
    } else {
      free(data_);
      data_ = NULL;
    }
    dsize_ = left;
  }

 protected:
  ParseState state_;
  std::vector<IsoBox*> sub_boxs;
  uint32_t bsize_;
  uint32_t type_;
  uint32_t parsed_;
  // unparsed data
  uint32_t dsize_;
  uint8_t* data_;
};

class FullIsoBox : public IsoBox {
 public:
  FullIsoBox(uint8_t* pkt, size_t length) : IsoBox(pkt, length) {
    assert(length >= 12);
    full_ver_ = pkt[8];
    full_flags_ = 0;
    full_flags_ = pkt[9] << 16 | pkt[10] << 8 | pkt[11];
    parsed_ += 4;
  }

 protected:
  int8_t full_ver_;
  // bits 24
  int32_t full_flags_;
};

// clang-format off
// aligned(8) class FileTypeBox
// extends Box(‘ftyp’) {
//   unsigned int(32) major_brand;
//   unsigned int(32) minor_version;
//   unsigned int(32) compatible_brands[]; // to end of the box
// }
// clang-format on
class FtypeBox : public IsoBox {
 public:
  FtypeBox(uint8_t* pkt, size_t length) : IsoBox(pkt, length) {}

  void Parse() override {
    if (!IsAllArrive()) return;

    int k = 0;
    major_brand_ = MAKEBE32(data_[k++], data_[k++], data_[k++], data_[k++]);
    minor_version_ = MAKEBE32(data_[k++], data_[k++], data_[k++], data_[k++]);
    for (int i = 0; i < dsize_ - sizeof(major_brand_) - sizeof(minor_version_);
         i += 4) {
      compatible_brands.push_back(
          MAKEBE32(data_[k++], data_[k++], data_[k++], data_[k++]));
    }
    updateParse(dsize_);
  }

  std::string toString() override {
    char content[1024] = {0};
    int32_t size =
        snprintf(content, sizeof(content) - 1,
                 "major_brand=%c%c%c%c, minor_version=%d, compatible_brands=",
                 major_brand_ >> 24 & 0xFF, major_brand_ >> 16 & 0xFF,
                 major_brand_ >> 8 & 0xFF, major_brand_ & 0xFF, minor_version_);
    for (int i = 0; i < compatible_brands.size(); i++)
      size += snprintf(
          content + size, sizeof(content) - size - 1, "%c%c%c%c",
          compatible_brands[i] >> 24 & 0xFF, compatible_brands[i] >> 16 & 0xFF,
          compatible_brands[i] >> 8 & 0xFF, compatible_brands[i] & 0xFF);
    return std::string(content);
  }

 private:
  uint32_t major_brand_;
  uint32_t minor_version_;
  std::vector<uint32_t> compatible_brands;
};

// aligned(8) class MovieHeaderBox extends FullBox(‘mvhd’, version, 0) {
//
// if (version==1) {
//   unsigned int(64) creation_time;
//   unsigned int(64) modification_time;
//   unsigned int(32) timescale;
//   unsigned int(64) duration;
// } else { // version==0
//   unsigned int(32) creation_time;
//   unsigned int(32) modification_time;
//   unsigned int(32) timescale;
//   unsigned int(32) duration;
// }
//
// template int(32) rate = 0x00010000; // typically 1.0
// template int(16) volume = 0x0100; // typically, full volume
// const bit(16) reserved = 0;
// const unsigned int(32)[2] reserved = 0;
// template int(32)[9] matrix =
// { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 };
// // Unity matrix
// bit(32)[6] pre_defined = 0;
// unsigned int(32) next_track_ID;

class MvhdBox : public FullIsoBox {
 public:
  MvhdBox(uint8_t* pkt, size_t length) : FullIsoBox(pkt, length) {}

  void Parse() override {
    if (!IsAllArrive()) return;
    // scale is 1000
    duration_ms_ = MAKEBE32(data_[12], data_[13], data_[14], data_[15]);
    updateParse(dsize_);
  }

 private:
  uint32_t duration_ms_;
  uint32_t next_trackid_;
};

// aligned(8) class TrackHeaderBox
// extends FullBox(‘tkhd’, version, flags){
// if (version==1) {
//   unsigned int(64) creation_time;
//   unsigned int(64) modification_time;
//   unsigned int(32) track_ID;
//   const unsigned int(32) reserved = 0;
//   unsigned int(64) duration;
// } else { // version==0
//   unsigned int(32) creation_time;
//   unsigned int(32) modification_time;
//   unsigned int(32) track_ID;
//   const unsigned int(32) reserved = 0;
//   unsigned int(32) duration;
// }
// const unsigned int(32)[2] reserved = 0;
// template int(16) layer = 0;
// template int(16) alternate_group = 0;
// template int(16) volume = {if track_is_audio 0x0100 else 0};
// const unsigned int(16) reserved = 0;
// template int(32)[9] matrix=
// { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 };
// // unity matrix
// unsigned int(32) width;
// unsigned int(32) height;
// }
class TkhdBox : public FullIsoBox {
 public:
  TkhdBox(uint8_t* pkt, size_t length) : FullIsoBox(pkt, length) {}
  void Parse() override {
    if (!IsAllArrive()) return;

    track_id_ = MAKEBE32(data_[8], data_[9], data_[10], data_[11]);
    width_ = data_[dsize_ - 8] << 8 | data_[dsize_ - 7];
    height_ = data_[dsize_ - 4] << 8 | data_[dsize_ - 3];
    updateParse(dsize_);
  }

 private:
  uint32_t track_id_;
  uint32_t width_;
  uint32_t height_;
};

// aligned(8) class EditListBox extends FullBox(‘elst’, version, 0) {
// unsigned int(32) entry_count;
// for (i=1; i <= entry_count; i++) {
//   if (version==1) {
//     unsigned int(64) segment_duration;
//     int(64) media_time;
//   } else { // version==0
//     unsigned int(32) segment_duration;
//     int(32) media_time;
//   }
//   int(16) media_rate_integer;
//   int(16) media_rate_fraction = 0;
// }
//}
class ElstBox : public FullIsoBox {
 public:
  ElstBox(uint8_t* pkt, size_t length) : FullIsoBox(pkt, length) {}
  void Parse() override {
    if (!IsAllArrive()) return;

    updateParse(dsize_);
  }
};

class EdtsBox : public IsoBox {
 public:
  EdtsBox(uint8_t* pkt, size_t length) : IsoBox(pkt, length) {}
  void Parse() override {
    if (!IsAllArrive()) return;

    int32_t pkt_used = 0;
    IsoBox* sub_box = NULL;
    auto handle_box = [&] {
      pkt_used += sub_box->ParsedSize();
      int32_t box_used = sub_box->MoreData(data_ + pkt_used, dsize_ - pkt_used);
      pkt_used += box_used;
      sub_box->Parse();
      sub_boxs.push_back(sub_box);
    };

    while (pkt_used < dsize_) {
      switch (MAKEBE32(data_[pkt_used + 4], data_[pkt_used + 5],
                       data_[pkt_used + 6], data_[pkt_used + 7])) {
        case MAKEBE32('e', 'l', 's', 't'): {
          sub_box = new ElstBox(data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
        } break;

        default:
          printf("in edts %c%c%c%c\n", data_[pkt_used + 4], data_[pkt_used + 5],
                 data_[pkt_used + 6], data_[pkt_used + 7]);
          pkt_used = dsize_;
          break;
      }
    }
    updateParse(dsize_);
  }
};

class MdhdBox : public FullIsoBox {
 public:
  MdhdBox(uint8_t* pkt, size_t length) : FullIsoBox(pkt, length) {}
  void Parse() override {
    if (!IsAllArrive()) return;

    updateParse(dsize_);
  }
};

// aligned(8) class HandlerBox extends FullBox(‘hdlr’, version = 0, 0) {
//   unsigned int(32) pre_defined = 0;
//   unsigned int(32) handler_type;
//   const unsigned int(32)[3] reserved = 0;
//   string name;
// }
class HdlrBox : public FullIsoBox {
 public:
  HdlrBox(uint8_t* pkt, size_t length) : FullIsoBox(pkt, length) {}
  void Parse() override {
    if (!IsAllArrive()) return;

    printf("hdlr type %c%c%c%c\n", data_[4], data_[5], data_[6], data_[7]);
    type_ = MAKEBE32(data_[4], data_[5], data_[6], data_[7]);
    updateParse(dsize_);
  }

  uint32_t HandlerType() { return type_; }

 private:
  uint32_t type_;
};

class VmhdBox : public FullIsoBox {
 public:
  VmhdBox(uint8_t* pkt, size_t length) : FullIsoBox(pkt, length) {}
  void Parse() override {
    if (!IsAllArrive()) return;

    updateParse(dsize_);
  }
};

class SmhdBox : public FullIsoBox {
 public:
  SmhdBox(uint8_t* pkt, size_t length) : FullIsoBox(pkt, length) {}
  void Parse() override {
    if (!IsAllArrive()) return;

    updateParse(dsize_);
  }
};

class DrefBox : public FullIsoBox {
 public:
  DrefBox(uint8_t* pkt, size_t length) : FullIsoBox(pkt, length) {}
  void Parse() override {
    if (!IsAllArrive()) return;

    updateParse(dsize_);
  }
};

class DinfBox : public IsoBox {
 public:
  DinfBox(uint8_t* pkt, size_t length) : IsoBox(pkt, length) {}
  void Parse() override {
    if (!IsAllArrive()) return;

    int32_t pkt_used = 0;
    IsoBox* sub_box = NULL;
    auto handle_box = [&] {
      pkt_used += sub_box->ParsedSize();
      int32_t box_used = sub_box->MoreData(data_ + pkt_used, dsize_ - pkt_used);
      pkt_used += box_used;
      sub_box->Parse();
      sub_boxs.push_back(sub_box);
    };

    while (pkt_used < dsize_) {
      switch (MAKEBE32(data_[pkt_used + 4], data_[pkt_used + 5],
                       data_[pkt_used + 6], data_[pkt_used + 7])) {
        case MAKEBE32('d', 'r', 'e', 'f'): {
          sub_box = new DrefBox(data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
        } break;

        default:
          printf("unhandle in dinf: %c%c%c%c\n", data_[pkt_used + 4],
                 data_[pkt_used + 5], data_[pkt_used + 6], data_[pkt_used + 7]);
          pkt_used = dsize_;
          break;
      }
    }

    updateParse(dsize_);
  }
};

// aligned(8) abstract class SampleEntry (unsigned int(32) format)
// extends Box(format){
//   const unsigned int(8)[6] reserved = 0;
//   unsigned int(16) data_reference_index;
// }

// class AudioSampleEntry(codingname) extends SampleEntry (codingname){
//   const unsigned int(32)[2] reserved = 0;
//   template unsigned int(16) channelcount = 2;
//   template unsigned int(16) samplesize = 16;
//   unsigned int(16) pre_defined = 0;
//   const unsigned int(16) reserved = 0 ;
//   template unsigned int(32) samplerate = { default samplerate of media}<<16;
// }

// aligned(8) class SampleDescriptionBox (unsigned int(32) handler_type)
// extends FullBox('stsd', 0, 0){
// int i ;
// unsigned int(32) entry_count;
//   for (i = 1 ; i <= entry_count ; i++){
//     switch (handler_type){
//       case ‘soun’: // for audio tracks
//       AudioSampleEntry();
//       break;
//       case ‘vide’: // for video tracks
//       VisualSampleEntry();
//       break;
//       case ‘hint’: // Hint track
//       HintSampleEntry();
//       break;
//       case ‘meta’: // Metadata track
//       MetadataSampleEntry();
//       break;
//     }
//   }
// }
class StsdBox : public FullIsoBox {
 public:
  StsdBox(uint32_t handler_type, uint8_t* pkt, size_t length)
      : FullIsoBox(pkt, length) {
    handler_type_ = handler_type;
  }
  void Parse() override {
    if (!IsAllArrive()) return;

    int k = 0;
    uint32_t count = MAKEBE32(data_[k++], data_[k++], data_[k++], data_[k++]);
    switch (handler_type_) {
      case MAKEBE32('v', 'i', 'd', 'e'): {
        uint32_t vbox = MAKEBE32(data_[k++], data_[k++], data_[k++], data_[k++]);
        printf("stsd box %d count %d for vide, vbox %d %c%c%c%c\n", bsize_, count,
                        vbox,
                        data_[k], data_[k+ 1], data_[k+ 2], data_[k+ 3]);
      } break;
      case MAKEBE32('s', 'o', 'u', 'n'): {
        uint32_t sbox = MAKEBE32(data_[k++], data_[k++], data_[k++], data_[k++]);
        printf("stsd box %d count %d for soun sbox %d %c%c%c%c\n", bsize_, count,
                        sbox,
                        data_[k], data_[k+ 1], data_[k+ 2], data_[k+ 3]);
      } break;
    }

    updateParse(dsize_);
  }

 private:
  uint32_t handler_type_;
};

class StblBox : public IsoBox {
 public:
  StblBox(uint32_t handler_type, uint8_t* pkt, size_t length)
      : IsoBox(pkt, length) {
    handler_type_ = handler_type;
  }
  void Parse() override {
    if (!IsAllArrive()) return;

    int32_t pkt_used = 0;
    IsoBox* sub_box = NULL;
    auto handle_box = [&] {
      pkt_used += sub_box->ParsedSize();
      int32_t box_used = sub_box->MoreData(data_ + pkt_used, dsize_ - pkt_used);
      pkt_used += box_used;
      sub_box->Parse();
      sub_boxs.push_back(sub_box);
    };

    while (pkt_used < dsize_) {
      switch (MAKEBE32(data_[pkt_used + 4], data_[pkt_used + 5],
                       data_[pkt_used + 6], data_[pkt_used + 7])) {
        case MAKEBE32('s', 't', 's', 'd'): {
          sub_box =
              new StsdBox(handler_type_, data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
        } break;

        default:
          printf("unhandle in stbl: %c%c%c%c\n", data_[pkt_used + 4],
                 data_[pkt_used + 5], data_[pkt_used + 6], data_[pkt_used + 7]);
          pkt_used = dsize_;
          break;
      }
    }

    updateParse(dsize_);
  }

 private:
  uint32_t handler_type_;
};

class MinfBox : public IsoBox {
 public:
  MinfBox(uint32_t handler_type, uint8_t* pkt, size_t length)
      : IsoBox(pkt, length) {
    handler_type_ = handler_type;
  }
  void Parse() override {
    if (!IsAllArrive()) return;

    int32_t pkt_used = 0;
    IsoBox* sub_box = NULL;
    auto handle_box = [&] {
      pkt_used += sub_box->ParsedSize();
      int32_t box_used = sub_box->MoreData(data_ + pkt_used, dsize_ - pkt_used);
      pkt_used += box_used;
      sub_box->Parse();
      sub_boxs.push_back(sub_box);
    };

    while (pkt_used < dsize_) {
      switch (MAKEBE32(data_[pkt_used + 4], data_[pkt_used + 5],
                       data_[pkt_used + 6], data_[pkt_used + 7])) {
        case MAKEBE32('s', 'm', 'h', 'd'): {
          sub_box = new SmhdBox(data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
        } break;

        case MAKEBE32('v', 'm', 'h', 'd'): {
          sub_box = new VmhdBox(data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
        } break;

        case MAKEBE32('d', 'i', 'n', 'f'): {
          sub_box = new DinfBox(data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
        } break;

        case MAKEBE32('s', 't', 'b', 'l'): {
          sub_box =
              new StblBox(handler_type_, data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
        } break;

        default:
          printf("unhandle in minf: %c%c%c%c\n", data_[pkt_used + 4],
                 data_[pkt_used + 5], data_[pkt_used + 6], data_[pkt_used + 7]);
          pkt_used = dsize_;
          break;
      }
    }

    updateParse(dsize_);
  }

 private:
  uint32_t handler_type_;
};

class MdiaBox : public IsoBox {
 public:
  MdiaBox(uint8_t* pkt, size_t length) : IsoBox(pkt, length) {}
  void Parse() override {
    if (!IsAllArrive()) return;

    int32_t pkt_used = 0;
    IsoBox* sub_box = NULL;
    auto handle_box = [&] {
      pkt_used += sub_box->ParsedSize();
      int32_t box_used = sub_box->MoreData(data_ + pkt_used, dsize_ - pkt_used);
      pkt_used += box_used;
      sub_box->Parse();
      sub_boxs.push_back(sub_box);
    };

    while (pkt_used < dsize_) {
      switch (MAKEBE32(data_[pkt_used + 4], data_[pkt_used + 5],
                       data_[pkt_used + 6], data_[pkt_used + 7])) {
        case MAKEBE32('m', 'd', 'h', 'd'): {
          sub_box = new MdhdBox(data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
        } break;

        case MAKEBE32('h', 'd', 'l', 'r'): {
          sub_box = new HdlrBox(data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
          handler_type_ = (reinterpret_cast<HdlrBox*>(sub_box))->HandlerType();
        } break;

        case MAKEBE32('m', 'i', 'n', 'f'): {
          sub_box =
              new MinfBox(handler_type_, data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
        } break;

        default:
          printf("unhandle in mdia: %c%c%c%c\n", data_[pkt_used + 4],
                 data_[pkt_used + 5], data_[pkt_used + 6], data_[pkt_used + 7]);
          pkt_used = dsize_;
          break;
      }
    }

    updateParse(dsize_);
  }

 private:
  uint32_t handler_type_;
};

class TrakBox : public IsoBox {
 public:
  TrakBox(uint8_t* pkt, size_t length) : IsoBox(pkt, length) {}
  void Parse() override {
    if (!IsAllArrive()) return;

    int32_t pkt_used = 0;
    IsoBox* sub_box = NULL;
    auto handle_box = [&] {
      pkt_used += sub_box->ParsedSize();
      int32_t box_used = sub_box->MoreData(data_ + pkt_used, dsize_ - pkt_used);
      pkt_used += box_used;
      sub_box->Parse();
      sub_boxs.push_back(sub_box);
    };

    while (pkt_used < dsize_) {
      switch (MAKEBE32(data_[pkt_used + 4], data_[pkt_used + 5],
                       data_[pkt_used + 6], data_[pkt_used + 7])) {
        case MAKEBE32('t', 'k', 'h', 'd'): {
          sub_box = new TkhdBox(data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
        } break;

        case MAKEBE32('e', 'd', 't', 's'): {
          sub_box = new EdtsBox(data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
        } break;

        case MAKEBE32('m', 'd', 'i', 'a'): {
          sub_box = new MdiaBox(data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
        } break;

        default:
          printf("unhandle in trak: %c%c%c%c\n", data_[pkt_used + 4],
                 data_[pkt_used + 5], data_[pkt_used + 6], data_[pkt_used + 7]);
          pkt_used = dsize_;
          break;
      }
    }
    updateParse(dsize_);
  }
};

class UdtaBox : public IsoBox {
 public:
  UdtaBox(uint8_t* pkt, size_t length) : IsoBox(pkt, length) {}
  void Parse() override {
    if (!IsAllArrive()) return;

    updateParse(dsize_);
  }
};

class MoovBox : public IsoBox {
 public:
  MoovBox(uint8_t* pkt, size_t length) : IsoBox(pkt, length) {}
  void Parse() override {
    if (!IsAllArrive()) return;

    int32_t pkt_used = 0;
    IsoBox* sub_box = NULL;
    auto handle_box = [&] {
      pkt_used += sub_box->ParsedSize();
      int32_t box_used = sub_box->MoreData(data_ + pkt_used, dsize_ - pkt_used);
      pkt_used += box_used;
      sub_box->Parse();
      sub_boxs.push_back(sub_box);
    };

    while (pkt_used < dsize_) {
      switch (MAKEBE32(data_[pkt_used + 4], data_[pkt_used + 5],
                       data_[pkt_used + 6], data_[pkt_used + 7])) {
        case MAKEBE32('m', 'v', 'h', 'd'): {
          sub_box = new MvhdBox(data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
        } break;

        case MAKEBE32('t', 'r', 'a', 'k'): {
          sub_box = new TrakBox(data_ + pkt_used, dsize_ - pkt_used);
          handle_box();

        } break;

        case MAKEBE32('u', 'd', 't', 'a'): {
          sub_box = new UdtaBox(data_ + pkt_used, dsize_ - pkt_used);
          handle_box();
        } break;
      }
    }

    updateParse(dsize_);
  }

  std::string toString() override {
    char str[1024] = {0};
    snprintf(str, sizeof(str) - 1, "moov box, size=%d", bsize_);
    return std::string(str);
  }
};

class MdatBox : public IsoBox {
 public:
  MdatBox(uint8_t* pkt, size_t length) : IsoBox(pkt, length) {}
  void Parse() override {
    // TODO
    updateParse(dsize_);
  }

  std::string toString() override {
    char str[1024] = {0};
    snprintf(str, sizeof(str) - 1, "mdat box, size=%d", bsize_);
    return std::string(str);
  }
};

class FreeBox : public IsoBox {
 public:
  FreeBox(uint8_t* pkt, size_t length) : IsoBox(pkt, length) {}
  void Parse() override { updateParse(dsize_); }

  std::string toString() override {
    char str[1024] = {0};
    snprintf(str, sizeof(str) - 1, "free box, size=%d", bsize_);
    return std::string(str);
  }
};

int main() {
  FILE* file = fopen("./test.mp4", "r");
  IsoBox* currentbox = NULL;
  while (1) {
    uint8_t packets[4096] = {0};
    int32_t pkt_used = 0;
    int32_t readed = fread(packets, 1, sizeof(packets), file);
    if (readed <= 0) {
      printf("read file OVER\n");
      break;
    }

    if (currentbox != NULL && !currentbox->IsAllArrive()) {
      int32_t box_used =
          currentbox->MoreData(packets, sizeof(packets) - pkt_used);
      pkt_used += box_used;

      currentbox->Parse();
      if (currentbox->IsAllArrive()) {
        printf("%s\n", currentbox->toString().c_str());
        delete currentbox;
        currentbox = NULL;
      }

      if (pkt_used >= readed) {
        continue;
      }
    }

    auto handle_box = [&] {
      pkt_used += currentbox->ParsedSize();

      int32_t box_used =
          currentbox->MoreData(packets + pkt_used, sizeof(packets) - pkt_used);
      pkt_used += box_used;
      currentbox->Parse();
      if (currentbox->IsAllArrive()) {
        printf("%s\n", currentbox->toString().c_str());
        delete currentbox;
        currentbox = NULL;
      }
    };

    while (pkt_used < readed) {
      switch (MAKEBE32(packets[pkt_used + 4], packets[pkt_used + 5],
                       packets[pkt_used + 6], packets[pkt_used + 7])) {
        case MAKEBE32('f', 't', 'y', 'p'): {
          currentbox =
              new FtypeBox(packets + pkt_used, sizeof(packets) - pkt_used);
          handle_box();
          break;
        }

        case MAKEBE32('f', 'r', 'e', 'e'): {
          currentbox =
              new FreeBox(packets + pkt_used, sizeof(packets) - pkt_used);
          handle_box();
          break;
        }

        case MAKEBE32('m', 'd', 'a', 't'): {
          currentbox =
              new MdatBox(packets + pkt_used, sizeof(packets) - pkt_used);
          handle_box();
          break;
        }

        case MAKEBE32('m', 'o', 'o', 'v'): {
          currentbox =
              new MoovBox(packets + pkt_used, sizeof(packets) - pkt_used);
          handle_box();
          break;
        }
      }
    }
  }

  fclose(file);

  return 0;
}

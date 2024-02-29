#include <stdio.h>
#include <string.h>
#include "bits_operation.h"

// Article:
// https://zhuanlan.zhihu.com/p/608579756

// ISO/IEC
//
// 13818-1

// 2.4.3 Specification of the Transport Stream syntax and semantics

// Annex F Graphics of Syntax for this Recommendation | International Standard

#define TS_PACKETS_LEN 188
#define PAT_PID 0

struct TSContext {
  uint32_t pmt_pid;
};

// table_id 8 uimsbf
// section_syntax_indicator 1 bslbf
// '0' 1 bslbf
// reserved 2 bslbf
// section_length 12 uimsbf
// transport_stream_id 16 uimsbf
// reserved 2 bslbf
// version_number 5 uimsbf
// current_next_indicator 1 bslbf
// section_number 8 uimsbf
// last_section_number 8 uimsbf
// for (i = 0; i < N; i++) {
//   program_number 16 uimsbf
//   reserved 3 bslbf
//   if (program_number == '0') {
//     network_PID 13 uimsbf
//   }
//   else {
//     program_map_PID 13 uimsbf
//   }
// }
// CRC_32 32 rpchof
void ParsePAT(uint8_t *packets, TSContext* context) {
  uint32_t index = 0;
  assert(packets[index++] == 0x00);

  BeBitsReader reader1(&packets[index], 2);
  index += 2;
  reader1.readBits(1);
  reader1.readBits(1);
  reader1.readBits(2);
  uint32_t length = reader1.readBits(12);
  uint32_t stream_id = packets[index++] << 8 | packets[index++];
  // skip
  index += 3;
  for (int i = 0; i < (length - 5 - 4 /* CRC */) / 4; i++) {
    uint32_t program_number = packets[index++] << 8 | packets[index++];
    BeBitsReader reader(&packets[index], 2);
    index += 2;
    reader.readBits(3);
    uint32_t PID = reader.readBits(13);
    if (PID >= 0x1FFF /* null packet */) {
      break;
    }
    if (program_number != 0x00) {
      context->pmt_pid = PID;
    }
  }
}

// table_id 8 uimsbf
// section_syntax_indicator 1 bslbf
// '0' 1 bslbf
// reserved 2 bslbf
// section_length 12 uimsbf
// program_number 16 uimsbf
// reserved 2 bslbf
// version_number 5 uimsbf
// current_next_indicator 1 bslbf
// section_number 8 uimsbf
// last_section_number 8 uimsbf
// reserved 3 bslbf
// PCR_PID 13 uimsbf
// reserved 4 bslbf
// program_info_length 12 uimsbf
// for (i = 0; i < N; i++) {
//   descriptor()
// }
// for (i = 0; i < N1; i++) {
//   stream_type 8 uimsbf
//   reserved 3 bslbf
//   elementary_PID 13 uimsbf
//
//   reserved 4 bslbf
//   ES_info_length 12 uimsbf
//   for (i = 0; i < N2; i++) {
//     descriptor()
//   }
// }
// CRC_32 32 rpchof
void ParsePMT(uint8_t *packets, TSContext* context) {
  uint32_t index = 0;
  assert(packets[index++] == 0x02);

  BeBitsReader reader1(&packets[index], 2);
  index += 2;
  reader1.readBits(1);
  reader1.readBits(1);
  reader1.readBits(2);
  uint32_t length = reader1.readBits(12);

  uint32_t program_number = packets[index++] << 8 | packets[index++];
  printf("program_number %d\n", program_number);

}

int main() {

  FILE* file = fopen("./test.ts", "r");
  TSContext context;
  memset(&context, 0, sizeof(TSContext));

  int x = 0;
  while(1) {
    uint8_t packets[TS_PACKETS_LEN] = {0};
    size_t readed = fread(packets, 1, sizeof(packets), file);
    if (readed != TS_PACKETS_LEN) {
      break;
    }
    assert(packets[0] = 0x47);
    // sync_byte 8 bslbf
    // transport_error_indicator 1 bslbf
    // payload_unit_start_indicator 1 bslbf
    // transport_priority 1 bslbf
    // PID 13 uimsbf
    // transport_scrambling_control 2 bslbf
    // adaptation_field_control 2 bslbf
    // continuity_counter 4 uimsbf
    BeBitsReader reader(&packets[1], 3);
    reader.readBits(1);
    uint8_t start_indi = reader.readBits(1);
    reader.readBits(1);
    uint32_t pid = reader.readBits(13);
    reader.readBits(2);
    uint32_t field_control = reader.readBits(2);
    reader.readBits(4);
    if (pid == PAT_PID) {
      if (start_indi == 1) {
        uint8_t bytes_to_section = packets[4];
        ParsePAT(&packets[5 + bytes_to_section], &context);
      } else {
        ParsePAT(&packets[4], &context);
      }
    } else if (pid == context.pmt_pid) {
      if (start_indi == 1) {
        uint8_t bytes_to_section = packets[4];
        ParsePMT(&packets[5 + bytes_to_section], &context);
      } else {
        ParsePMT(&packets[4], &context);
      }
    }

    x++;
    if (x == 3)
      break;
  }
  fclose(file);

  return 0;
}

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>

#define BYTE_BTIS 8
class BeBitsWriter {
 public:
  BeBitsWriter(uint8_t bytes)
      : bytes_(bytes), write_byte_pos_(0), write_bit_pos_(0) {
    data_ = (uint8_t*)malloc(bytes_);
  }

  ~BeBitsWriter() {
    if (data_) {
      free(data_);
    }
  }

  void writeBits(uint32_t bits, uint32_t value) {
    if (BYTE_BTIS - write_bit_pos_ >= bits) {
      data_[write_byte_pos_] |= (value << (BYTE_BTIS - write_bit_pos_ - bits));
      write_bit_pos_ += bits;
    } else {
      // 1 write write_byte_pos_
      uint32_t left_bits = bits - (BYTE_BTIS - write_bit_pos_);
      uint32_t val1 = (value >> left_bits) & (0xFF >> write_bit_pos_);
      data_[write_byte_pos_] |= val1;
      // 2 write_byte_pos_ + 1
      write_byte_pos_++;
      write_bit_pos_ = 0;

      uint32_t val2 = value & (0xFF >> (BYTE_BTIS - left_bits));
      data_[write_byte_pos_] |=
          (val2 << (BYTE_BTIS - write_bit_pos_ - left_bits));
      write_bit_pos_ += left_bits;
    }

    if (write_bit_pos_ == BYTE_BTIS) {
      write_byte_pos_++;
      write_bit_pos_ = 0;
    }
  }

  uint8_t* data_;

 private:
  uint8_t bytes_;
  uint32_t write_byte_pos_;
  uint32_t write_bit_pos_;
};

class BeBitsReader {
 public:
  BeBitsReader(uint8_t* data, uint8_t length)
      : data_(data), bytes_(length), read_byte_pos_(0), read_bit_pos_(0) {}

  uint32_t readBits(uint32_t bits) {
    uint32_t result = 0;
    if (bits <= (BYTE_BTIS - read_bit_pos_)) {
      uint32_t val =
          (data_[read_byte_pos_] >> (BYTE_BTIS - read_bit_pos_ - bits)) &
          (0xFF >> (BYTE_BTIS - bits));
      read_bit_pos_ += bits;
      result = val;
    } else {
      // 1 read read_byte_pos_
      uint32_t left = bits - (BYTE_BTIS - read_bit_pos_);
      uint32_t val1 =
          data_[read_byte_pos_] & (0xFF >>  read_bit_pos_);

      // 2 read read_byte_pos_ + 1
      read_byte_pos_++;
      read_bit_pos_ = 0;
      uint32_t val2 = (data_[read_byte_pos_] >> (BYTE_BTIS - left)) &
                      (0xFF >> (BYTE_BTIS - left));
      read_bit_pos_ += left;

      result = (val1 << left) | val2;
    }
    if (read_bit_pos_ == BYTE_BTIS) {
      read_byte_pos_++;
      read_bit_pos_ = 0;
    }
    return result;
  }

  uint8_t* data_;

 private:
  uint8_t bytes_;
  uint32_t read_byte_pos_;
  uint32_t read_bit_pos_;
};


// void testWriter() {
//   BeBitsWriter writer(2);
//   writer.writeBits(10, 0x201);
//   writer.writeBits(6, 0x13);
//   assert(writer.data_[0] == 0x80 && writer.data_[1] == 0x53);
// }
//
// void testReader1() {
//   uint8_t value[1] = {0xAD};
//   BeBitsReader reader(value, 1);
//   uint32_t v1 = reader.readBits(3);
//   uint32_t v2 = reader.readBits(5);
//   assert(v1 == 0x05 && v2 == 0x0D);
// }
//
// void testReader2() {
//   uint8_t value[2] = {0x25, 0x2A};
//   BeBitsReader reader(value, 2);
//   uint32_t v1 = reader.readBits(3);
//   uint32_t v2 = reader.readBits(8);
//   uint32_t v3 = reader.readBits(5);
//   assert(v1 == 0x01 && v2 == 0x29 && v3 == 0x0A);
// }
//
// void testReader3() {
//   uint8_t value[2] = {0xFF, 0xFF};
//   BeBitsReader reader(value, 2);
//   uint32_t v1 = reader.readBits(3);
//   uint32_t v2 = reader.readBits(8);
//   uint32_t v3 = reader.readBits(5);
//   assert(v1 == 0x07 && v2 == 0xFF && v3 == 0x1F);
// }


#include "TestCoder.h"
#include <stdio.h>
#include <deque>

void TestCoder::setUp() {
    printf("TestCoder::setUp \n");
}

// given arrays contain two kind character, find minimum steps to move two kind to different side
// https://www.bilibili.com/video/BV1vi4y1R7g9?p=3&vd_source=97b4bf98c5b1c5e6ff06f5ab921071f7
int getMinStepsToAdjustElement()(std::vector<char> array) {
  char target_char = array[0];
  int32_t target_char_count = 1;
  int32_t another_char_count = 0;
  int32_t total_steps = 0;
  // now check move all 'target_char' to left side
  for (char check : array) {
    if (check == target_char) {
        total_steps += another_char_count;
        target_char_count ++;
    } else {
        another_char_count++;
    }
  }
  return total_steps;
}

// how many node can integer 'k' cover given 'array'
// https://www.bilibili.com/video/BV1vi4y1R7g9?p=2&vd_source=97b4bf98c5b1c5e6ff06f5ab921071f7
int findIndexFromArray(std::vector<int32_t> array, int32_t from, int32_t to, int32_t k) {
  // check tail and head
  if (k < array[0] || k > array[array.size() - 1])
    return 0;

  if (from - to <= 1) {
    return k > array[to] ? to : from;
  }

  if (k > array[(from - to) / 2]) {
    return findIndexFromArray(array, (from - to) / 2, to, k);
  } else {
    return findIndexFromArray(array, from, (from - to) / 2, k);
  }
}

// https://en.wikipedia.org/wiki/Dynamic_programming

void TestCoder::run() {

}

void TestCoder::tearDown() {
    printf("TestCoder::tearDown \n\n");
}

//
//  TestScale.cpp
//  DemoTerm
//
//  Created by shiruiwei on 2020/8/24.
//  Copyright © 2020 shiruiwei. All rights reserved.
//

#include "TestScale.h"

#include <stdio.h>
#include <vector>

// Round |value_to_round| to a multiple of |multiple|. Prefer rounding upwards,
// but never more than |max_value|.
int roundUp(int value_to_round, int multiple, int max_value) {
  const int rounded_value =
      (value_to_round + multiple - 1) / multiple * multiple;
  return rounded_value <= max_value ? rounded_value
                                    : (max_value / multiple * multiple);
}

struct Fraction {
  int numerator;
  int denominator;

  // Determines number of output pixels if both width and height of an input of
  // |input_pixels| pixels is scaled with the fraction numerator / denominator.
  int scale_pixel_count(int input_pixels) {
    return (numerator * numerator * input_pixels) / (denominator * denominator);
  }
};

Fraction FindScale(int input_pixels, int target_pixels, int max_pixels) {

  // Don't scale up original.
  if (target_pixels >= input_pixels)
    return Fraction{1, 1};

  Fraction current_scale = Fraction{1, 1};
  Fraction best_scale = Fraction{1, 1};
  // The minimum (absolute) difference between the number of output pixels and
  // the target pixel count.
  int min_pixel_diff = std::numeric_limits<int>::max();
  if (input_pixels <= max_pixels) {
    // Start condition for 1/1 case, if it is less than max.
    min_pixel_diff = std::abs(input_pixels - target_pixels);
  }
  printf("min_pixel_diff %d\n", min_pixel_diff);

  // Alternately scale down by 2/3 and 3/4. This results in fractions which are
  // effectively scalable. For instance, starting at 1280x720 will result in
  // the series (3/4) => 960x540, (1/2) => 640x360, (3/8) => 480x270,
  // (1/4) => 320x180, (3/16) => 240x125, (1/8) => 160x90.
  while (current_scale.scale_pixel_count(input_pixels) > target_pixels) {
    if (current_scale.numerator % 3 == 0 &&
        current_scale.denominator % 2 == 0) {
      // Multiply by 2/3.
      current_scale.numerator /= 3;
      current_scale.denominator /= 2;
      printf("current scale %dx%d multiply 3/2\n", current_scale.numerator, current_scale.denominator);
    } else {
      // Multiply by 3/4.
      current_scale.numerator *= 3;
      current_scale.denominator *= 4;
        printf("current scale %dx%d multiply 3/4\n", current_scale.numerator, current_scale.denominator);
    }

    int output_pixels = current_scale.scale_pixel_count(input_pixels);
    if (output_pixels <= max_pixels) {
      int diff = std::abs(target_pixels - output_pixels);
      printf("output_pixels %d max_pixels %d diff %d min_diff %d\n",
             output_pixels, max_pixels, diff, min_pixel_diff);
      if (diff < min_pixel_diff) {
        min_pixel_diff = diff;
        best_scale = current_scale;
      }
    }
    printf("target %d current %d\n", target_pixels, current_scale.scale_pixel_count(input_pixels));
  }

  return best_scale;
}

void TestSacle::setUp() {
    printf("TestSacle::setUp \n");
}

void TestSacle::run() {

  int align = 32;

  int inW = 720;
  int inH = 1280;
  int outW = 176;
  int outH = 312;

  const float requested_aspect = outW / static_cast<float>(outH);
  int cropW = std::min(inW, static_cast<int>(inH * requested_aspect));
  int cropH = std::min(inH, static_cast<int>(inW / requested_aspect));

  printf("== in %dx%d request %dx%d crop %dx%d\n", inW, inH, outW, outH, cropW, cropH);

  Fraction outFraction = FindScale(cropW * cropH, outW * outH, outW * outH);
  printf("== out fraction %dx%d\n", outFraction.numerator, outFraction.denominator);

  int cropWithRound =
    roundUp(cropW,
            outFraction.denominator * align, inW);
  int cropHeightRound =
      roundUp(cropH,
              outFraction.denominator * align, inH);
  printf("== cropWithRound %d cropHeightRound %d\n", cropWithRound, cropHeightRound);

  printf("== result %dx%d\n",
         cropWithRound / outFraction.denominator * outFraction.numerator,
         cropHeightRound /  outFraction.denominator * outFraction.numerator);
}

void TestSacle::tearDown() {
    printf("TestSacle::tearDown \n\n");
}

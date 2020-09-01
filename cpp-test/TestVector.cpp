//
//  TestVector.cpp
//  DemoTerm
//
//  Created by shiruiwei on 2020/8/9.
//  Copyright © 2020 shiruiwei. All rights reserved.
//

#include "TestVector.h"
#include <stdio.h>
#include <vector>

void TestVector::setUp() {
    printf("\n");
}

void TestVector::run() {
    std::vector<int> v;

    printf("%s strat\n", __func__);
    timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    int i(0);
    while (i++ < 100000) {
        v.push_back(i);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &t2);

    long _costms = 0;
    long _costs = 0;
    if (t2.tv_nsec < t1.tv_nsec) {
        _costms = (t2.tv_nsec + (1000000000 - t1.tv_nsec)) / 1000000;
        _costs = t2.tv_sec -1 - t1.tv_sec;
    } else {
        _costms = (t2.tv_nsec - t1.tv_nsec) / 1000000;
        _costs = t2.tv_sec - t1.tv_sec;
    }

    printf("%s over cost %ds %dms\n", __func__, _costs, _costms);

}

void TestVector::tearDown() {
    printf("\n");
}


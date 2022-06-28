//
//  main.cpp
//  DemoTerm
//
//  Created by shiruiwei on 2020/3/1.
//  Copyright © 2020 shiruiwei. All rights reserved.
//

#include <time.h>
#include <stdio.h>
#include "TestVector.h"
#include "TestScale.h"
#include "TestSet.h"
#include "TestSlot.h"
#include "TestDeque.h"
#include "TestGLM.h"
#include "TestTimestampExtrapolator.h"
#include "TestJMP.h"
#include "TestMap.h"
#include "TestPtr.h"
#include "TestJson.h"

#define TEST_VECTOR false
#define TEST_SCALE false
#define TEST_SET false
#define TEST_SLOT false
#define TEST_SLOT false
#define TEST_QUEUE false
#define TEST_QUEUE false
#define TEST_GLM false
#define TEST_GLM false
#define TEST_TIME_EXTRAPOLATOR false
#define TEST_JMP false
#define TEST_MAP false
#define TEST_PTR false

int main() {

    if (TEST_VECTOR) {
        TestVector testVecotr;
        testVecotr.setUp();
        testVecotr.run();
        testVecotr.tearDown();
    }

    if (TEST_SCALE) {
        TestSacle testScale;
        testScale.setUp();
        testScale.run();
        testScale.tearDown();
    }

    if (TEST_SET) {
        TestSet testSet;
        testSet.setUp();
        testSet.run();
        testSet.tearDown();
    }

    if (TEST_SLOT) {
        TestSlot testSlot;
        testSlot.setUp();
        testSlot.run();
        testSlot.tearDown();
    }

    if (TEST_QUEUE) {
        TestDeque testDeque;
        testDeque.setUp();
        testDeque.run();
        testDeque.tearDown();
    }

    if (TEST_GLM) {
        TestGLM testGLM;
        testGLM.setUp();
        testGLM.run();
        testGLM.tearDown();
    }

    if (TEST_TIME_EXTRAPOLATOR) {
        TimestampExtrapolator testTimestamp(390012248);
        testTimestamp.setUp();
        testTimestamp.run();
        testTimestamp.tearDown();
    }

    if (TEST_JMP) {
        TestJMP testJMP;
        testJMP.setUp();
        testJMP.run();
        testJMP.tearDown();
    }

    if (TEST_MAP) {
        TestMap testMap;
        testMap.setUp();
        testMap.run();
        testMap.tearDown();
    }

    if (TEST_PTR) {
        TestPtr testPtr;
        testPtr.setUp();
        testPtr.run();
        testPtr.tearDown();
    }

    TestJson testJson;
    testJson.setUp();
    testJson.run();
    testJson.tearDown();

    return 0;
}

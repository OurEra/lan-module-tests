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

int main() {

    TestVector testVecotr;
    testVecotr.setUp();
    testVecotr.run();
    testVecotr.tearDown();

    TestSacle testScale;
    testScale.setUp();
    testScale.run();
    testScale.tearDown();

    TestSet testSet;
    testSet.setUp();
    testSet.run();
    testSet.tearDown();

    TestSlot testSlot;
    testSlot.setUp();
    testSlot.run();
    testSlot.tearDown();

    TestDeque testDeque;
    testDeque.setUp();
    testDeque.run();
    testDeque.tearDown();

    TestGLM testGLM;
    testGLM.setUp();
    testGLM.run();
    testGLM.tearDown();

    TimestampExtrapolator testTimestamp(390012248);
    testTimestamp.setUp();
    testTimestamp.run();
    testTimestamp.tearDown();

    TestJMP testJMP;
    testJMP.setUp();
    testJMP.run();
    testJMP.tearDown();

    TestMap testMap;
    testMap.setUp();
    testMap.run();
    testMap.tearDown();

    TestPtr testPtr;
    testPtr.setUp();
    testPtr.run();
    testPtr.tearDown();

    TestJson testJson;
    testJson.setUp();
    testJson.run();
    testJson.tearDown();

    return 0;
}

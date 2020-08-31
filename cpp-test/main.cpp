//
//  main.cpp
//  DemoTerm
//
//  Created by shiruiwei on 2020/3/1.
//  Copyright © 2020 shiruiwei. All rights reserved.
//

#include <stdio.h>
#include "TestVector.h"
#include "TestScale.h"

int main() {
    
    TestVector testVecotr;
    testVecotr.setUp();
    testVecotr.run();
    testVecotr.tearDown();
    
    TestSacle testScale;
    testScale.setUp();
    testScale.run();
    testScale.tearDown();
    
    return 0;
}

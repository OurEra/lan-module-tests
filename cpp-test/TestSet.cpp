#include "TestSet.h"
#include <stdio.h>
#include <vector>
#include <set>

void TestSet::setUp() {
    printf("TestSet::setUp \n");
}

void TestSet::run() {
    std::set<int> testSet;
    testSet.insert(10);
    testSet.insert(20);
    testSet.insert(30);
    testSet.insert(30);

    std::set<int>::iterator it;
    for (it = testSet.begin(); it != testSet.end(); it++) {
        printf("test set value %d\n", *it);
    }

    std::multiset<int> testMultiSet;
    testMultiSet.insert(10);
    testMultiSet.insert(20);
    testMultiSet.insert(30);
    testMultiSet.insert(30);

    std::multiset<int>::iterator mit;
    for (mit = testMultiSet.begin(); mit != testMultiSet.end(); mit++) {
        printf("test multi set value %d\n", *mit);
    }

}

void TestSet::tearDown() {
    printf("TestSet::tearDown \n\n");
}


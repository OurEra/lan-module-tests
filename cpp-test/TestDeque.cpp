#include "TestDeque.h"
#include <stdio.h>
#include <deque>

void TestDeque::setUp() {
    printf("TestDeque::setUp \n");
}

void TestDeque::run() {
    std::deque<int> testDeque;

    testDeque.push_back(30);
    testDeque.push_back(30);
    testDeque.push_back(40);
    testDeque.push_back(40);

    testDeque.push_front(50);
    testDeque.push_front(50);
    testDeque.push_front(60);
    testDeque.push_front(60);

    std::deque<int>::iterator it;
    for (it = testDeque.begin(); it != testDeque.end(); it++) {
        printf("dequeu value %d\n", *it);
    }
    printf("dequeu size %d\n", testDeque.size());
}

void TestDeque::tearDown() {
    printf("TestDeque::tearDown \n\n");
}

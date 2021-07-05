#include "TestPtr.h"
#include <iostream>
#include <memory>
#include <list>
#include <map>
#include <type_traits>
#include <utility>
#include <typeinfo>

class testPtr {
public:
    size_t aaa;
    size_t bbb;
};

using testPtrList = std::list<std::unique_ptr<testPtr>>;


void TestPtr::setUp() {
    printf("TestPtr::setUp \n");
}

void TestPtr::run() {
    testPtrList prtList1;
    testPtrList prtList2;
    std::unique_ptr<testPtr> test_ins(new testPtr());

    printf("ptr %p\n", test_ins.get());
    prtList1.push_back(std::move(test_ins));
    printf("ptr %p\n", test_ins.get());
    prtList2.push_back(std::move(test_ins));
    printf("ptr %p\n", test_ins.get());
    printf("ptr list1 %d list2 %d\n", prtList1.size(), prtList1.size());

    for (auto& it : prtList1) {
      printf("type [ %s ]\n", typeid(it).name());
    }
}

void TestPtr::tearDown() {
    printf("TestPtr::tearDown \n\n");
}


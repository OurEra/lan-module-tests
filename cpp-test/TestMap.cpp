#include "TestMap.h"
#include <iostream>
#include <map>
#include <utility>

typedef std::pair<std::string, std::string> pair;

struct comp
{
    template<typename T>
    bool operator()(const T &l, const T &r) const
    {
        if (l.first == r.first) {
            return l.second > r.second;
        }

        return l.first < r.first;
    }
};

void TestMap::setUp() {
    printf("TestMap::setUp \n");
}

void TestMap::run() {

    std::map<pair,int, comp> map =
    {
        { std::make_pair("C++", "C++14"), 2014 },
        { std::make_pair("C++", "C++17"), 2017 },
        { std::make_pair("Java", "Java 7"), 2011 },
        { std::make_pair("Java", "Java 8"), 2014 },
        { std::make_pair("C", "C11"), 2011 }
    };

    for (const auto &entry: map)
    {
        auto key_pair = entry.first;
        std::cout << "{" << key_pair.first << "," << key_pair.second << "}, "
                  << entry.second << std::endl;
    }

}

void TestMap::tearDown() {
    printf("TestMap::tearDown \n\n");
}


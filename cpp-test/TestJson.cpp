#include "TestJson.h"
#include "base/json.hpp"
#include <iostream>

using json = nlohmann::json;

// https://github.com/nlohmann/json
void TestJson::setUp() {
    printf("TestJson::setUp \n");
}

void TestJson::run() {

    json dtls = json::parse(R"(
                   {
                       "fingerprints": [
                           {
                               "algorithm": "sha-256",
                               "value": "9D:58:E2:29:F8:5B:61:FE:C1:73:3E:DF:6A:4A:18:21:25:90:DC:F6:33:B0:42:1A:54:9A:20:99:FA:54:56:F8"
                           }
                       ],
                       "role": "client"
                   }
         )");

    std::string algo;
    // usei 'at' to access item
    // array support range-based loop
    // use 'get_to'/'get' to get value
    for (auto& el : dtls.at("fingerprints")) {
        // get_to
        el.at("algorithm").get_to(algo);
        // get
        std::string value = el.at("value").get<std::string>();
        std::cout << "algo: " << algo << " value: " << value << std::endl;
    }

    // dump directly
    std::cout << "\ndump:\n" << dtls.dump(2) << std::endl;

    // use 'push_back' to put item in array
    json item;
    item["ssrcs"] = json::array();
    item["ssrcs"].push_back({
        {"attribute", "msid"},
        {"id", 233546658},
    });

    std::cout << "\ndump:\n" << item.dump(2) << std::endl;

    // use 'contains' to check item exist
    std::cout << "contain ssrcs " << item.contains("ssrcs") << std::endl;

    // for every json item
    for (auto& el : item.items()) {
        std::cout << el.key() << " : " << el.value() << std::endl;
        if (el.value().is_array()) {
          std::cout << "is array " << el.value().is_array()
                  << "; is object " << el.value().is_object()
                  << "; is string " << el.value().is_string() << std::endl;
        }
    }
}

void TestJson::tearDown() {
    printf("TestJson::tearDown \n\n");
}


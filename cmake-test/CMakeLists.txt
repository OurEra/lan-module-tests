cmake_minimum_required(VERSION 3.4.1)
message("cmake version: ${CMAKE_VERSION}")

# build application's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


add_library(hello-cmake SHARED
            hello-cmake.cpp)

target_link_libraries(hello-cmake
                      android
                      log)

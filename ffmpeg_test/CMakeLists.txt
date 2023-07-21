project("ffmpeg_test")
cmake_minimum_required(VERSION 3.4.1)
message("cmake version: ${CMAKE_VERSION}")

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -g")
include_directories(${CMAKE_SOURCE_DIR} ./include)

#"-framework AudioToolbox"
#"-framework CoreAudio"
#"-framework VideoToolbox"
#"-framework CoreFoundation"
#"-framework CoreMedia"
#"-framework CoreVideo"
#"-framework CoreServices"
#  iconv

add_executable(ffmpeg_test test_encode_avc.cpp)
target_link_libraries(ffmpeg_test PRIVATE lzma z m c bz2 dl
                     "-Wl,--whole-archive"
                     ${CMAKE_SOURCE_DIR}/lib/libx264.a
                     ${CMAKE_SOURCE_DIR}/lib/libavcodec.a
                     ${CMAKE_SOURCE_DIR}/lib/libavformat.a
                     ${CMAKE_SOURCE_DIR}/lib/libavutil.a
                     ${CMAKE_SOURCE_DIR}/lib/libswresample.a
                     "-Wl,--no-whole-archive")

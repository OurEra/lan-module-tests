cmake_minimum_required(VERSION 3.4.1)
project(webrtc-AEC3-demo)
               
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-exceptions -fno-rtti")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-DWEBRTC_POSIX -DWEBRTC_LINUX)

include_directories(${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/base
        ${CMAKE_CURRENT_LIST_DIR}/audio_processing
        ${CMAKE_CURRENT_LIST_DIR}/base/abseil
        ${CMAKE_CURRENT_LIST_DIR}/base/jsoncpp/include)

list(APPEND API_SOURCES
  api/echo_canceller3_config.cc
  api/echo_canceller3_factory.cc
  api/echo_canceller3_config_json.cc
)

list(APPEND PROCESS_SOURCES
    audio_processing/channel_layout.cc
    audio_processing/high_pass_filter.cc
    audio_processing/audio_frame.cc
    audio_processing/audio_buffer.cc
    audio_processing/splitting_filter.cc
    audio_processing/splitting_filter_c.c
    audio_processing/three_band_filter_bank.cc
    audio_processing/logging/apm_data_dumper.cc
    audio_processing/sparse_fir_filter.cc
    audio_processing/utility/cascaded_biquad_filter.cc
    audio_processing/utility/ooura_fft.cc
    audio_processing/utility/ooura_fft_sse2.cc

    audio_processing/resampler/push_sinc_resampler.cc
    audio_processing/resampler/sinc_resampler.cc
    audio_processing/resampler/sinc_resampler_neon.cc
    audio_processing/resampler/sinc_resampler_sse.cc
)

aux_source_directory(audio_processing/aec3/ PROCESS_SOURCES)

list(APPEND BASE_SOURCES
  base/rtc_base/memory/aligned_malloc.cc
  base/rtc_base/checks.cc
  base/rtc_base/critical_section.cc
  base/rtc_base/race_checker.cc
  base/rtc_base/logging.cc
  base/rtc_base/strings/json.cc
  base/rtc_base/string_encode.cc
  base/rtc_base/platform_thread_types.cc
  base/rtc_base/time_utils.cc
  base/rtc_base/string_utils.cc
  base/rtc_base/strings/string_builder.cc

  base/system_wrappers/source/metrics.cc
  base/system_wrappers/source/field_trial.cc
  base/system_wrappers/source/cpu_features.cc
)

add_subdirectory(base/jsoncpp)
add_subdirectory(base/abseil)

add_executable(aec3-demo
  ${API_SOURCES}
  ${PROCESS_SOURCES}
  ${BASE_SOURCES}
  demo/demo.cc
  demo/wavreader.c
  demo/wavwriter.c
)

target_link_libraries(aec3-demo
        ${CMAKE_CURRENT_BINARY_DIR}/base/jsoncpp/lib/libjsoncpp.a
        ${CMAKE_CURRENT_BINARY_DIR}/lib/libabsl_absl_bad_optional_access.a
        ${CMAKE_CURRENT_BINARY_DIR}/lib/libabsl_absl_throw_delegate.a
        ${CMAKE_CURRENT_BINARY_DIR}/lib/libabsl_absl_strings.a
        ${CMAKE_CURRENT_BINARY_DIR}/lib/libabsl_absl_raw_logging_internal.a
        dl m pthread)

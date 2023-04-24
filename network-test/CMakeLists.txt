cmake_minimum_required(VERSION 3.4.1)
message("cmake version: ${CMAKE_VERSION}")
set(CMAKE_C_STANDARD 99)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/kcp
                    ${CMAKE_SOURCE_DIR}/libev)
link_directories(${CMAKE_SOURCE_DIR})

list(APPEND NETWORK_SRC
  network.c
  ${CMAKE_SOURCE_DIR}/kcp_wrap.c
  ${CMAKE_SOURCE_DIR}/kcp/ikcp.c
)

## tcp-kcp client 
add_executable(ev_client local_tcp_client.c ${NETWORK_SRC})
target_link_libraries(ev_client pthread
                      ${CMAKE_SOURCE_DIR}/libev/.libs/libev.a)

## tcp-kcp server 
add_executable(ev_server local_tcp_server.c ${NETWORK_SRC}
        ${CMAKE_SOURCE_DIR}/libev-examples/src/array-heap.c)
target_include_directories(ev_server PUBLIC ${CMAKE_SOURCE_DIR}/libev-examples/include)
target_link_libraries(ev_server pthread
                      ${CMAKE_SOURCE_DIR}/libev/.libs/libev.a)

## udp-kcp client 
add_executable(ev_udp_client udp_file_client.c ${NETWORK_SRC})
target_link_libraries(ev_udp_client pthread
                      ${CMAKE_SOURCE_DIR}/libev/.libs/libev.a)


## udp-kcp server 
add_executable(ev_udp_server udp_file_server.c ${NETWORK_SRC})
target_include_directories(ev_udp_server PUBLIC ${CMAKE_SOURCE_DIR}/libev-examples/include)
target_link_libraries(ev_udp_server pthread
                      ${CMAKE_SOURCE_DIR}/libev/.libs/libev.a)

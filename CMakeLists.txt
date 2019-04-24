cmake_minimum_required(VERSION 3.13)
project(dns_req C)

set(CMAKE_C_STANDARD 11)

add_executable(dns_req main.c)

if(WIN32)
    target_link_libraries(dns_req wsock32 ws2_32)
endif()
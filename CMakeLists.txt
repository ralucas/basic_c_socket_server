cmake_minimum_required(VERSION 3.3)
project(basic_c_socket_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    server.c server.h)

add_executable(basic_c_socket_server ${SOURCE_FILES})
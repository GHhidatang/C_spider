link_libraries(ws2_32)
cmake_minimum_required(VERSION 3.15)
project(1_30)

set(CMAKE_CXX_STANDARD 14)

add_executable(1_30 main.cpp http.cpp http.h)
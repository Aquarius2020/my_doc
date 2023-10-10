cmake_minimum_required(VERSION 3.17)
project(tmp)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 20)

add_executable(tmp main.cpp)
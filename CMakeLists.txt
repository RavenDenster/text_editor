cmake_minimum_required(VERSION 3.25)
project(rope)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SRCFILES "*.cpp")

add_executable(rope main.cpp)
target_sources(rope PRIVATE ${SRCFILES})
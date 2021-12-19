cmake_minimum_required(VERSION 3.4)
project(csapp3e-allocator)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wno-unused-variable -Wno-sign-compare")

add_executable(memlib memlib.cc)
add_executable(mm mm.cc)
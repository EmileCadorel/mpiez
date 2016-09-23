CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
set(CMAKE_CXX_COMPILER "mpic++")
project(mpiez)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")


add_executable(mpiez main.cc)
include_directories(${CMAKE_SOURCE_DIR}/../)

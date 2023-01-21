cmake_minimum_required(VERSION 3.0)
project(HuffmanCoding CXX)
set(CMAKE_SOURCE_DIR src/)
include_directories(lib/)
add_executable(
    compressor
    ${CMAKE_SOURCE_DIR}/types.cpp
    ${CMAKE_SOURCE_DIR}/serialization.cpp
    ${CMAKE_SOURCE_DIR}/compressor.cpp
)
add_executable(
    decompressor
    ${CMAKE_SOURCE_DIR}/decompressor.cpp
    ${CMAKE_SOURCE_DIR}/types.cpp
)
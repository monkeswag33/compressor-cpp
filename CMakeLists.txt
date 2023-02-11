cmake_minimum_required(VERSION 3.1)
project(HuffmanCoding CXX)
set(CMAKE_CXX_STANDARD 17)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    message(STATUS "Enabling tests...")
    add_subdirectory(tests)
endif()
add_library(compression STATIC
    src/compression.cpp
    src/decompression.cpp
    src/types.cpp
    src/serialization.cpp
    src/util.cpp
)
target_include_directories(compression PUBLIC lib/)
add_executable(compressor src/compressor.cpp)
target_link_libraries(compressor compression)
add_executable(decompressor src/decompressor.cpp)
target_link_libraries(decompressor compression)

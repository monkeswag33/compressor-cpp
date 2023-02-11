cmake_minimum_required(VERSION 3.15)
project(HuffmanCoding CXX)
set(CMAKE_CXX_STANDARD 17)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
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

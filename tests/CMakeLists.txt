configure_file(data/OneFile.txt ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
add_executable(TestOneFile TestOneFile.cpp)
target_link_libraries(TestOneFile compression)
add_test(NAME TestOneFile COMMAND TestOneFile)
file(GLOB TEST_FILES GLOB "data/*")
file(COPY ${TEST_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_executable(
    TestOneFile
    src/TestOneFile.cpp
    src/util.cpp
)
add_executable(
    TestDirectory
    src/TestDirectory.cpp
    src/util.cpp
)
target_include_directories(TestOneFile PRIVATE lib/)
target_include_directories(TestDirectory PRIVATE lib/)
target_link_libraries(TestOneFile compression)
target_link_libraries(TestDirectory compression)
set_property(TARGET TestOneFile APPEND PROPERTY ADDITIONAL_CLEAN_FILES OneFile.cmp OneFile.txt.dcmp)
set_property(TARGET TestDirectory APPEND PROPERTY ADDITIONAL_CLEAN_FILES directory.cmp directory1/)
add_test(NAME TestOneFile COMMAND TestOneFile)
add_test(NAME TestDirectory COMMAND TestDirectory)
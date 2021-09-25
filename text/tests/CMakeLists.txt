find_package(GTest CONFIG REQUIRED)
add_executable(texttests
    textdoc-tests.cpp
)

target_link_libraries(texttests PRIVATE GTest::gtest GTest::gtest_main libtext::text)
include(GoogleTest)
gtest_discover_tests(texttests WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

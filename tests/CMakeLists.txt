# Add test sources
cmake_minimum_required(VERSION 4.0)
set(TEST_SOURCES
    test_convert_to_datetime.cpp
    test_get_release_dt.cpp
)

# Create the test executable
add_executable(test_runner test_runner.cpp ${TEST_SOURCES})

# Link Google Test and the main library
target_link_libraries(test_runner gtest gtest_main pthread bloomberg-geco-connector)

# Enable testing
add_test(NAME AllTests COMMAND test_runner)

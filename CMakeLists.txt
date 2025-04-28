cmake_minimum_required(VERSION 3.16)

# Identification
project(blpapi-connector VERSION 0.0.1 LANGUAGES CXX)

# Standards
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD REQUIRED True)

# Directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
file(GLOB_RECURSE EXAMPLES_SOURCES "examples/*.cpp")

# Create a static library
add_library(blpapi-connector STATIC ${SOURCES} ${HEADERS})
target_include_directories(blpapi-connector PUBLIC include)
target_link_libraries(blpapi-connector PUBLIC blpapi)
# Add the library to the build
add_library(blpapi-connector::blpapi-connector ALIAS blpapi-connector)
# Add the library to the build

cmake_minimum_required(VERSION 3.16)

project(blpconn VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fPIE")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

set(BLPAPI3_64_URL "https://blpapi.bloomberg.com/download/releases/raw/files/blpapi_cpp_3.25.3.1-linux.tar.gz")
set(BLPAPI3_64_DIR "${CMAKE_SOURCE_DIR}/blpapi3_64")
set(BLPAPI3_64_TAR "${CMAKE_SOURCE_DIR}/blpapi3_64.tar.gz")

# Check if blpapi3_64 directory exists
if(NOT EXISTS ${BLPAPI3_64_TAR})
    execute_process(
        COMMAND curl -f -o blpapi3_64.tar.gz ${BLPAPI3_64_URL}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE DOWNLOAD_RESULT
    )

    if(NOT ${DOWNLOAD_RESULT} EQUAL "0")
        message(FATAL_ERROR "Failed to download blpapi3_64 from ${BLPAPI3_64_URL}")
    endif()
endif()

if(NOT EXISTS ${BLPAPI3_64_DIR})
    # Create the blpapi3_64 directory
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E make_directory ${BLPAPI3_64_DIR}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE MKDIR_RESULT
    )
    if(NOT ${MKDIR_RESULT} EQUAL "0")
        message(FATAL_ERROR "Failed to create directory ${BLPAPI3_64_DIR}")
    endif()

    # Extract the library
    execute_process(
        COMMAND tar xf blpapi3_64.tar.gz -C ${BLPAPI3_64_DIR}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE MKDIR_RESULT
    )
    if(NOT ${EXTRACT_RESULT} EQUAL "0")
        message(FATAL_ERROR "Failed to extract blpapi3_64.tar.gz")
    endif()
else()
    message(STATUS "blpapi3_64 already exists in ${BLPAPI3_64_DIR}, skipping download and extraction")
endif()

link_directories(${BLPAPI3_64_DIR}/blpapi_cpp_3.25.3.1/Linux)

set(BLPAPILIB "${BLPAPI3_64_DIR}/blpapi_cpp_3.25.3.1/Linux/libblpapi3_64.so")

file(GLOB SOURCE_FILES src/*.cpp)  # This already includes serialize.cpp
add_library(blpconn STATIC ${SOURCE_FILES})

target_include_directories(blpconn PRIVATE include)
# target_link_libraries(blpconn PRIVATE ${BLPAPI3_64_DIR}/blpapi_cpp_3.25.3.1/Linux/libblpapi3_64.so)
target_link_libraries(blpconn PRIVATE ${BLPAPILIB})
install(TARGETS blpconn DESTINATION lib)

file(GLOB EXAMPLE_FILES examples/*.cpp)
foreach(example ${EXAMPLE_FILES})
    get_filename_component(binname ${example} NAME_WE)
    add_executable(${binname} ${example})
    target_include_directories(${binname} PRIVATE include)
    target_link_libraries(${binname} PRIVATE boost_system readline blpconn ${BLPAPILIB})
    install(TARGETS ${binname} DESTINATION bin)
    add_dependencies(${binname} blpconn)
endforeach(example ${EXAMPLE_FILES})

# Find Qt6
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gtest_SOURCE_DIR}/googletest/include ${gtest_SOURCE_DIR}/googlemock/include)

file(GLOB TEST_FILES tests/*.cpp)
foreach(test ${TEST_FILES})
    get_filename_component(binname ${test} NAME_WE)
    add_executable(${binname} ${test})
    target_include_directories(${binname} PRIVATE include)
    target_link_libraries(${binname} PRIVATE blpconn ${BLPAPILIB} gtest gmock pthread)
    install(TARGETS ${binname} DESTINATION bin)
    add_dependencies(${binname} blpconn)
    add_test(NAME ${binname} COMMAND ${binname})
endforeach(test ${TEST_FILES})


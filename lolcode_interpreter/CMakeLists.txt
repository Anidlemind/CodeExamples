cmake_minimum_required(VERSION 3.22)

project(LOLCODE)

find_package(BISON 3.5 REQUIRED)
find_package(FLEX REQUIRED)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(HEADERS_DIR ${CMAKE_SOURCE_DIR}/headers)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

set(HEADERS
        ${HEADERS_DIR}/enums.h
        ${HEADERS_DIR}/variable.h
        ${HEADERS_DIR}/statement.h
    )

BISON_TARGET(MyParser
        ${CMAKE_SOURCE_DIR}/parser.y
        ${CMAKE_SOURCE_DIR}/parser.cpp
        DEFINES_FILE ${CMAKE_SOURCE_DIR}/parser.hh
)

FLEX_TARGET(MyScanner
        ${CMAKE_SOURCE_DIR}/scanner.l
        ${CMAKE_SOURCE_DIR}/scanner.cpp
)

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(LOLCODE
        ${SOURCE_DIR}/main.cpp
        ${SOURCE_DIR}/driver.cpp
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS}
        ${HEADERS}
        ${SOURCE_DIR}/enums.cpp
        ${SOURCE_DIR}/variable.cpp
        ${SOURCE_DIR}/statement.cpp)

target_include_directories(LOLCODE PRIVATE ${CMAKE_SOURCE_DIR})

add_custom_target(clear
        COMMAND echo ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND rm -rf ${CMAKE_SOURCE_DIR}/build/*
        COMMAND rm -rf ${CMAKE_SOURCE_DIR}/scanner.cpp
        COMMAND rm -rf ${CMAKE_SOURCE_DIR}/parser.cpp
        COMMAND rm -rf ${CMAKE_SOURCE_DIR}/parser.hh
        COMMAND rm -rf ${CMAKE_SOURCE_DIR}/location.hh
        )

add_custom_target(test
        COMMAND echo "--------   starting simple   -------"
	COMMAND ./LOLCODE ${TEST_DIR}/test_simple.in
	COMMAND echo "--------    starting math    -------"
        COMMAND ./LOLCODE ${TEST_DIR}/test_math.in
	COMMAND echo "--------    starting bool    -------"
        COMMAND ./LOLCODE ${TEST_DIR}/test_bool.in
	COMMAND echo "-------- starting comparison -------"
        COMMAND ./LOLCODE ${TEST_DIR}/test_comparison.in
	COMMAND echo "--------     starting if     -------"
        COMMAND ./LOLCODE ${TEST_DIR}/test_if.in
	COMMAND echo "--------    starting loop    -------"
        COMMAND ./LOLCODE ${TEST_DIR}/test_cycles.in
        )




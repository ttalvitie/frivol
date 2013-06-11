cmake_minimum_required(VERSION 2.6) 
project(frivol_test)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

find_package(Boost 1.44 COMPONENTS unit_test_framework REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${frivol_test_SOURCE_DIR}/..)

add_definitions(
	-std=c++11
	-Wall
)

file(
	GLOB TEST_SOURCES
	perftest.cpp
)
add_executable(test ${TEST_SOURCES})

target_link_libraries(test ${Boost_LIBRARIES})

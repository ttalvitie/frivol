cmake_minimum_required(VERSION 2.6) 
project(frivol_test)

find_package(Boost 1.44 COMPONENTS unit_test_framework REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${frivol_test_SOURCE_DIR}/..)

add_definitions(
	-std=c++11
	-DBOOST_TEST_DYN_LINK
)

file(
	GLOB TEST_SOURCES
	test.cpp
	containers/array.cpp
	containers/priority_queue.cpp
	containers/search_tree.cpp
	containers/stack.cpp
	containers/dynamic_array.cpp
	fortune/fortune_algorithm.cpp
	fortune/beach_line.cpp
	geometry_traits_float.cpp 
)
add_executable(test ${TEST_SOURCES})

target_link_libraries(test ${Boost_LIBRARIES})

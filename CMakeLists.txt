cmake_minimum_required(VERSION 2.8)
project(tcp)

set(CXX_DEBUG "-Wall -Wextra -g -D_GLIBCXX_DEBUG -fsanitize=undefined")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_DEBUG} -std=c++11 ")

include_directories(include)
set(SOURCES 	)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME})

find_package(GTest)

set(TEST_SOURCES 	src/test/gtest_main.cc)

if(GTEST_FOUND)
	add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})
	target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} ${GTEST_LIBRARIES})
endif(GTEST_FOUND)

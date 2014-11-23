cmake_minimum_required(VERSION 2.8)
project(tcp)

set(CXX_DEBUG "-Wall -Wextra -g -D_GLIBCXX_DEBUG -fsanitize=undefined")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_DEBUG} -std=c++11 ")

include_directories(include)
set(SOURCES     include/tcp/tcp.hh
                include/tcp/async/c/epoll.hh
                include/tcp/async/client.hh
                include/tcp/async/io_event.hh
                include/tcp/async/io_service.hh
                include/tcp/async/server.hh
                include/tcp/util/address.hh
                include/tcp/util/async_value.hh
                include/tcp/util/buffer.hh
                include/tcp/util/job.hh
                include/tcp/util/nothing.hh

                src/tcp/async/c/epoll.cc
                src/tcp/async/client.cc
                src/tcp/async/io_event.cc
                src/tcp/async/io_service.cc
                src/tcp/async/server.cc
                src/tcp/util/address.cc
                src/tcp/util/buffer.cc)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME})

find_package(GTest)

set(TEST_SOURCES 	src/test/gtest_main.cc
                    src/test/tcp/async/c/epoll_test.cc
                    src/test/tcp/async/echo_test.cc
                    src/test/tcp/util/address_test.cc
                    src/test/tcp/util/buffer_test.cc)

if(GTEST_FOUND)
	add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})
	target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} ${GTEST_LIBRARIES})
endif(GTEST_FOUND)

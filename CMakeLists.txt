cmake_minimum_required(VERSION 3.7.2)
project(path)

include_directories(include)
file(GLOB SOURCES "src/*.c")
file(GLOB TEST_SOURCES "tests/*.c")

# add_library(path_lib STATIC ${SOURCES})
add_executable(test_path ${SOURCES} ${TEST_SOURCES})

cmake_minimum_required(VERSION 3.7.2)
project(path)

include_directories(include)
file(GLOB SOURCES "src/*.c")

# add_library(path_lib STATIC ${SOURCES})
add_executable(path ${SOURCES})

project(jsmn++)
cmake_minimum_required(VERSION 2.8)

include(libjsmn.cmake)

set(SRC test.cpp)

add_executable(jsmn++ ${SRC})
target_link_libraries(jsmn++ libjsmn)

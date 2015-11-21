# libjsmn

include_directories(${CMAKE_CURRENT_LIST_DIR})

set(SRC ${CMAKE_CURRENT_LIST_DIR}/jsmn.h
        ${CMAKE_CURRENT_LIST_DIR}/jsmn.c
        ${CMAKE_CURRENT_LIST_DIR}/jsmn.hpp
        ${CMAKE_CURRENT_LIST_DIR}/jsmn.cpp)

add_library(libjsmn STATIC ${SRC})

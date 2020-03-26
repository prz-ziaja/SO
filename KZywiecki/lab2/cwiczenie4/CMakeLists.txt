cmake_minimum_required(VERSION 3.10)

project(cwiczenie4)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/time")

add_executable(copy copy3.c)

target_link_libraries(copy Time)

target_include_directories(copy PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/time")
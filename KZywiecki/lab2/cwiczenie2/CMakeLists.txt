cmake_minimum_required(VERSION 3.10)

file(GLOB src_files "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

project(cwiczenie2)
add_executable(cwiczenie2 ${src_files})
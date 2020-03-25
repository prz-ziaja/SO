cmake_minimum_required(VERSION 3.10)

file(GLOB src_files "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

project(cwiczenie1)
add_executable(cwiczenie1 ${src_files})
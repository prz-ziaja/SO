cmake_minimum_required(VERSION 3.10)


project(Cwiczenie1)

file(GLOB src_files "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
"${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_executable(Cwiczenie1 ${src_files})

include_directories(Cwiczenie1 ${CMAKE_CURRENT_SOURCE_DIR})

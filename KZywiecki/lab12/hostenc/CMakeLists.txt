cmake_minimum_required(VERSION 3.10)

project(proj)

add_executable(info hostinfo.c)
add_executable(gethostbyname gethostbyname.c)
add_executable(showip showip.c)

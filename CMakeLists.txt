cmake_minimum_required(VERSION 3.10)

project(cfetch)

include_directories(inc)
add_executable(cfetch src/main.c src/fetchline.c)

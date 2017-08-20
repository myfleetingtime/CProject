cmake_minimum_required(VERSION 3.6.2)
project(CProject)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c dorm.h)
add_executable(CProject ${SOURCE_FILES} dorm.h)
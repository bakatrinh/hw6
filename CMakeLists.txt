cmake_minimum_required(VERSION 3.8)
project(hw6)

set(CMAKE_C_STANDARD 99)

FILE(GLOB MyCSources *.c)
ADD_EXECUTABLE(hw6 ${MyCSources})
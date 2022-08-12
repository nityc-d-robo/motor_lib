cmake_minimum_required(VERSION 3.16.3)
project(motor_lib CXX)

include_directories(
  include
)

link_directories(
  include
)

add_library(md_lib SHARED
  src/md_lib.cpp
)

add_library(motor_lib STATIC
  src/motor_lib.cpp
)

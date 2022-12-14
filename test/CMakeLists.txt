cmake_minimum_required(VERSION 3.16.3)
project(test_smd CXX)

set(CMAKE_CXX_FLAGS "-fopenmp")

find_package(serial_connect REQUIRED)
find_package(motor_lib REQUIRED)

include_directories(
	include
)

add_executable(test_smd test_smd.cpp)

target_link_libraries(test_smd
	serial_connect
	motor_lib
	md_lib
	smd_lib
	sd_lib
	sr_lib
)
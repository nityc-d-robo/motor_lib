cmake_minimum_required(VERSION 3.16.3)
project(test_smd CXX)

set(CMAKE_CXX_FLAGS "-fopenmp")

find_package(serial_connect REQUIRED)
find_package(motor_lib REQUIRED)

include_directories(
	include
)

add_executable(test_md src/test_md.cpp)
add_executable(test_smd src/test_smd.cpp)

target_link_libraries(test_md
	serial_connect
	motor_lib
	md_lib
	smd_lib
	sd_lib
	sr_lib
)
target_link_libraries(test_smd
	serial_connect
	motor_lib
	md_lib
	smd_lib
	sd_lib
	sr_lib
)
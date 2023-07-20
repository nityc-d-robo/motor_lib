cmake_minimum_required(VERSION 3.16.3)
project(motor_sample CXX)

find_package(usb_connect REQUIRED)
find_package(motor_lib REQUIRED)

include_directories(
	include
)

add_executable(md_sample src/md_sample.cpp)

target_link_libraries(md_sample
	usb_connect
	motor_lib
	md_lib
	sd_lib
	smd_lib
	sm_lib
	sr_lib
)

add_executable(sd_sample src/sd_sample.cpp)

target_link_libraries(sd_sample
	usb_connect
	motor_lib
	md_lib
	sd_lib
	smd_lib
	sm_lib
	sr_lib
)

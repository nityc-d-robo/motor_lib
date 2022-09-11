cmake_minimum_required(VERSION 3.16.3)
project(libsr_lib CXX)

include_directories(
	include
)

link_directories(
	include
)

add_library(sr_lib SHARED
	src/sr_lib.cpp
)

add_library(motor_lib::sr_lib ALIAS
	sr_lib
)

set_target_properties(sr_lib PROPERTIES
	PUBLIC_HEADER include/sr_lib.hpp
)

install(TARGETS sr_lib
	EXPORT motor_lib-export
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION include/motor_lib
	PUBLIC_HEADER DESTINATION include/motor_lib/sr_lib
)

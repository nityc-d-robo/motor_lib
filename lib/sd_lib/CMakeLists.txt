cmake_minimum_required(VERSION 3.16.3)
project(libsd_lib CXX)

include_directories(
	include
)

link_directories(
	include
)

add_library(sd_lib SHARED
	src/sd_lib.cpp
)

add_library(motor_lib::sd_lib ALIAS
	sd_lib
)

set_target_properties(sd_lib PROPERTIES
	PUBLIC_HEADER include/sd_lib.hpp
)

install(TARGETS sd_lib
	EXPORT motor_lib-export
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION include/motor_lib
	PUBLIC_HEADER DESTINATION include/motor_lib/sd_lib
)

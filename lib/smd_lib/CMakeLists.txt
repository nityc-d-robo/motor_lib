cmake_minimum_required(VERSION 3.16.3)
project(libsmd_lib CXX)

include_directories(
	include
	../common_lib/include
)

link_directories(
	include
	../common_lib/include
)

add_library(smd_lib SHARED
	src/smd_lib.cpp
)

add_library(motor_lib::smd_lib ALIAS
	smd_lib
)

set_target_properties(smd_lib PROPERTIES
	PUBLIC_HEADER include/smd_lib.hpp
)

install(TARGETS smd_lib
	EXPORT motor_lib-export
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION include/motor_lib
	PUBLIC_HEADER DESTINATION include/motor_lib/smd_lib
)

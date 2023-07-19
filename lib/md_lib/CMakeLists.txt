cmake_minimum_required(VERSION 3.16.3)
project(libmd_lib CXX)

include_directories(
	include
	../common_lib/include
)

link_directories(
	include
	../common_lib/include
)

add_library(md_lib SHARED
	src/md_lib.cpp
)

add_library(motor_lib::md_lib ALIAS
	md_lib
)

set_target_properties(md_lib PROPERTIES
	PUBLIC_HEADER include/md_lib.hpp
)

install(TARGETS md_lib
	EXPORT motor_lib-export
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION include/motor_lib
	PUBLIC_HEADER DESTINATION include/motor_lib/md_lib
)

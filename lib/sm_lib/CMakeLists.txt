cmake_minimum_required(VERSION 3.16.3)
project(libsm_lib CXX)

include_directories(
	include
	../common_lib/include
)

link_directories(
	include
	../common_lib/include
)

add_library(sm_lib SHARED
	src/sm_lib.cpp
)

add_library(motor_lib::sm_lib ALIAS
	sm_lib
)

set_target_properties(sm_lib PROPERTIES
	PUBLIC_HEADER include/sm_lib.hpp
)

install(TARGETS sm_lib
	EXPORT motor_lib-export
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION include/motor_lib
	PUBLIC_HEADER DESTINATION include/motor_lib/sm_lib
)

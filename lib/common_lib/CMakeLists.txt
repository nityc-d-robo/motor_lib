cmake_minimum_required(VERSION 3.16.3)
project(libcommon_lib CXX)

include_directories(
	include
)

link_directories(
	include
)

add_library(common_lib SHARED
	src/common_lib.cpp
)

add_library(motor_lib::common_lib ALIAS
	common_lib
)

set_target_properties(common_lib PROPERTIES
	PUBLIC_HEADER include/common_lib.hpp
)

install(TARGETS common_lib
	EXPORT motor_lib-export
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION include/motor_lib
	PUBLIC_HEADER DESTINATION include/motor_lib/common_lib
)

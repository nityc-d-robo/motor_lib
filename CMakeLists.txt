cmake_minimum_required(VERSION 3.16.3)
project(motor_lib CXX)

set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

find_package(usb_connect REQUIRED)

add_subdirectory(lib/md_lib)
add_subdirectory(lib/sd_lib)
add_subdirectory(lib/smd_lib)
add_subdirectory(lib/sr_lib)
add_subdirectory(lib/sm_lib)
add_subdirectory(lib/common_lib)

add_definitions(-DMOTOR_INSTALL)

include_directories(
	include
	lib/md_lib/include
	lib/sd_lib/include
	lib/smd_lib/include
	lib/sr_lib/include
	lib/sm_lib/include
	lib/common_lib/include
)

link_directories(
	include
	lib/md_lib/include
	lib/sd_lib/include
	lib/smd_lib/include
	lib/sr_lib/include
	lib/sm_lib/include
	lib/common_lib/include
)

add_library(motor_lib SHARED
	src/motor_lib.cpp
)

set_target_properties(motor_lib PROPERTIES
	PUBLIC_HEADER include/motor_lib.hpp
)

target_link_libraries(motor_lib PUBLIC
	md_lib
	sd_lib
	smd_lib
	sr_lib
	sm_lib
	common_lib
)

install(TARGETS motor_lib
	EXPORT motor_lib-export
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION include
	PUBLIC_HEADER DESTINATION include/motor_lib
)

install(
	EXPORT motor_lib-export
	DESTINATION cmake
	NAMESPACE motor_lib::
	FILE motor_lib-config.cmake
)

export(EXPORT motor_lib-export
	FILE motor_lib-export.cmake
)

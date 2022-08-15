cmake_minimum_required(VERSION 3.16.3)
project(motor_lib CXX)

set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

find_package(serial_connect REQUIRED)

include_directories(
	include
)

link_directories(
	include
)

add_library(md_lib STATIC
	src/md_lib.cpp
)

add_library(motor_lib SHARED
	src/motor_lib.cpp
)

set_target_properties(motor_lib PROPERTIES
	PUBLIC_HEADER include/motor_lib.hpp
)

target_link_libraries(motor_lib
	md_lib
)

install(TARGETS motor_lib
	EXPORT motor_lib-export
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION include
	PUBLIC_HEADER DESTINATION include/motor_lib
)

install(TARGETS md_lib
	EXPORT md_lib-export
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION include
	PUBLIC_HEADER DESTINATION include/motor_lib
)

install(EXPORT motor_lib-export
	FILE motor_lib-config.cmake
	DESTINATION share/cmake/motor_lib/
	EXPORT_LINK_INTERFACE_LIBRARIES
)

install(EXPORT md_lib-export
	FILE md_lib-config.cmake
	DESTINATION share/cmake/motor_lib/
	EXPORT_LINK_INTERFACE_LIBRARIES
)

export(EXPORT motor_lib-export
	FILE motor_lib-export.cmake
)

export(EXPORT md_lib-export
	FILE md_lib-export.cmake
)

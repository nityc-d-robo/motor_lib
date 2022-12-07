#include <common_lib.hpp>

#include <iostream>

void MotorLib::__attribute__((weak)) errorMotor(std::string error_str_){
	std::cerr << "motor_lib error: " << error_str_ << std::endl;
}

void MotorLib::__attribute__((weak)) infoMotor(std::string info_str_){
	std::cout << "motor_lib info: " << info_str_ << std::endl;
}

#include <common_lib.hpp>

#include <iostream>

void __attribute__((weak)) MotorLib::errorMotor(std::string error_str_){
	std::cerr << "motor_lib error: " << error_str_ << std::endl;
}

void __attribute__((weak)) MotorLib::infoMotor(std::string info_str_){
	std::cout << "motor_lib info: " << info_str_ << std::endl;
}

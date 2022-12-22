#pragma once

#include <vector>
#include <array>

namespace MotorLib{
	typedef enum IdType{
		MD = 0x00, SD = 0x10, SMD = 0x20, BLMD = 0x30, SR = 0x40, SM = 0x50, EMMERGENCY = 0xf0
	}IdType;

	typedef enum Header{
		ADDRESS = 0, SEMI_ID, MODE
	}Header;

	typedef enum FinishStatus{
		STATUS = 0, SUCCESS, TIMEOUT, INTERRUPT, OTHER
	}FinishStatus;

	std::vector<std::array<uint8_t, 8> > finish_queue;		//return data queue
	constexpr uint8_t TX_SIZE = 10u;				//usb transmit size
	constexpr uint8_t RX_SIZE = 8u;					//usb receive size

	void __attribute__((weak)) errorMotor(std::string error_str_);
	void __attribute__((weak)) infoMotor(std::string info_str_);
};

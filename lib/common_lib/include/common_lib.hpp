#pragma once

#include <vector>

namespace MotorLib{
	typedef enum FinishStatus{
		STATUS, SUCCESS, TIMEOUT, INTERRUPT, OTHER
	}FinishStatus;

	typedef struct FinishData{
		uint8_t address;
		uint8_t semi_id;
		FinishStatus finish_status;
		uint8_t mode;
		float firmware;
		int32_t angle;
		bool lim_sw1;
	}FinishData;
	
	std::vector<FinishData> finish_queue;

	void __attribute__((weak)) errorMotor(std::string error_str_);
	void __attribute__((weak)) infoMotor(std::string info_str_);
};

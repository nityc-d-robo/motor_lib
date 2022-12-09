#pragma once

#include <vector>

namespace MotorLib{
	typedef enum IdType{
		MD = 0x00, SD = 0x10, SMD = 0x20, BLMD = 0x30, SR = 0x40, SM = 0x50, EMMERGENCY = 0xf0;
	}

	typedef enum FinishStatus{
		STATUS, SUCCESS, TIMEOUT, INTERRUPT, OTHER
	}FinishStatus;

	typedef struct StatusStruct{
		float firmware;
		int32_t angle;
		bool lim_sw0;
		bool lim_sw1;
	}StatusStruct;

	//store full color led's color
	typedef struct Color{
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	}Color;

	typedef struct SrStruct{
		bool voltage;
		float firmware;
		Color color;
		float freq;
	}

	typedef union DataPack{
		StatusStruct status;
		SrStruct sr_data;
		uint8_t mode;
	}

	typedef struct StatusData{
		uint8_t address;
		uint8_t semi_id;
		FinishStatus finish_status;
		DataPack datas;
	}StatusData;
	
	std::vector<StatusData> finish_queue;		//return data queue
	constexpr uint8_t TX_SIZE = 10u;		//usb transmit size
	constexpr uint8_t RX_SIZE = 8u;			//usb receive size

	void __attribute__((weak)) errorMotor(std::string error_str_);
	void __attribute__((weak)) infoMotor(std::string info_str_);
};

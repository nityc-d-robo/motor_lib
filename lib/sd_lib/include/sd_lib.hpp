#pragma once

#include <usb_connect/usb_connect.hpp>
#include <common_lib.hpp>

namespace MotorLib{
	typedef struct SdStatus{
		float firmware;
		bool lim_sw0, lim_sw1;
	}SdStatus;

	class Sd{
		public:
			typedef enum LimPort{
				PORT0, PORT1
			}LimPort;

			typedef enum OutPort{
				PORT0, PORT1
			}

			typedef enum Mode{
				STATUS, POWER, LIM_SW
			}Mode;

			Sd(UsbConnect& usb_);
			int sendPowers(uint8_t address_, uint16_t power1_, uint16_t power2_, uint32_t usb_timeout_);
			int sendPowers(uint8_t address_, uint8_t semi_id_, uint16_t power1_, uint16_t power2_, uint32_t usb_timeout_);
			int sendLimSw(uint8_t address_, OutPort out_port_, LimPort lim_port_, uint16_t first_power_, uint16_t next_power_, uint16_t timeout_, uint32_t usb_timeout_);
			int sendLimSw(uint8_t address_, uint8_t semi_id_, OutPort out_port_, LimPort lim_port_, uint16_t first_power_, uint16_t next_power_, uint16_t timeout_, uint32_t usb_timeout_);
			int sendStatus(uint8_t address_, StatusData& sd_status_, uint32_t usb_timeout_);
			int sendStatus(uint8_t address_, uint8_t semi_id_, SdStatus& sd_status_, uint32_t usb_timeout_);
		private:
			UsbConnect* usb;
	};
};

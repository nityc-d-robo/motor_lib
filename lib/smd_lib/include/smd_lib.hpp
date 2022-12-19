#pragma once

#include <usb_connect.hpp>

namespace MotorLib{
	class Smd{
		public:
			typedef struct SmdStatus{
				float firmware;
				uint8_t angle0, angle1;
			}SmdStatus;

			typedef enum OutPort{
				PORT0, PORT1
			};

			Smd(UsbConnect& usb_);
			int sendAngle(uint8_t address_, OutPort port_, uint8_t angle_, uint32_t usb_timeout_);
			int sendAngle(uint8_t address_, uint8_t semi_id_, OutPort port_, uint8_t angle_, uint32_t usb_timeout_);
			int sendAngleBoth(uint8_t address_, uint8_t angle0_, uint8_t angle1_, uint32_t usb_timeout_);
			int sendStatus(uint8_t address_);
		private:
			UsbConnect usb;
	};
};

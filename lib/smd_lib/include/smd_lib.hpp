#pragma once

#include <usb_connect/usb_connect.hpp>

namespace MotorLib{
	typedef struct SmdStatus{
		float firmware;
		uint8_t angle0, angle1;
	}SmdStatus;

	class Smd{
		public:
			typedef enum Port{
				PORT0, PORT1
			}OutPort;

			typedef enum Mode{
				STATUS, ANGLE, ANGLE_BOTH
			}Mode;

			Smd(UsbConnect& usb_);
			int sendAngle(uint8_t address_, Port port_, uint8_t angle_, uint32_t usb_timeout_);
			int sendAngle(uint8_t address_, uint8_t semi_id_, Port port_, uint8_t angle_, uint32_t usb_timeout_);
			int sendAngleBoth(uint8_t address_, uint8_t angle0_, uint8_t angle1_, uint32_t usb_timeout_);
			int sendAngleBoth(uint8_t address_, uint8_t semi_id_, uint8_t angle0_, uint8_t angle1_, uint32_t usb_timeout_);
			int sendStatus(uint8_t address_, SmdStatus& smd_status_, uint32_t usb_timeout_);
			int sendStatus(uint8_t address_, uint8_t semi_id_, SmdStatus& smd_status_, uint32_t usb_timeout_);
		private:
			UsbConnect* usb;
	};
};

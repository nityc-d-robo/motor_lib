#pragma once

#include <usb_connect/usb_connect.hpp>

namespace MotorLib{
	typedef struct MdStatus{
		uint8_t address;
		uint8_t semi_id;
		float firmware;
		int32_t angle;
		bool lim_sw1, lim_sw2;
	}MdStatus;

	class Md{
		public:
			Md(UsbConnect& usb_);
			int sendPwm(uint8_t address_, bool phase_, uint16_t power_, uint32_t usb_timeout_);
			int sendPwm(uint8_t address_, uint8_t semi_id_, bool phase_, uint16_t power_, uint32_t usb_timeout_);
			int sendSpeed(uint8_t address_, bool phase_, uint16_t speed_, uint16_t end_, uint16_t timeout_, uint32_t usb_timeout_);
			int sendSpeed(uint8_t address_, uint8_t semi_id_, bool phase_, uint16_t speed_, uint16_t end_, uint16_t timeout_, uint32_t usb_timeout_);
			int sendAngle(uint8_t address_, uint16_t speed_, int32_t angle_, uint16_t timeout_, uint32_t usb_timeout_);
			int sendAngle(uint8_t address_, uint8_t semi_id_, uint16_t speed_, int32_t angle_, uint16_t timeout_, uint32_t usb_timeout_);
			int sendLimSw(uint8_t address_, bool phase_, uint16_t speed_, bool port_, uint16_t timeout_, uint32_t usb_timeout_);
			int sendLimSw(uint8_t address_, uint8_t semi_id_, bool phase_, uint16_t speed_, bool port_, uint16_t timeout_, uint32_t usb_timeout_);
			int sendInit(uint8_t address_, bool angle_reset_, uint16_t max_rpm_, uint16_t max_torque_, uint16_t gear_ratio_, uint32_t usb_timeout_);
			int sendInit(uint8_t address_, uint8_t semi_id_, bool angle_reset_, uint16_t max_rpm_, uint16_t max_torque_, uint16_t gear_ratio_, uint32_t usb_timeout_);
			int sendStatus(uint8_t address_, MdStatus& md_status_, uint32_t usb_timeout_);
			int sendStatus(uint8_t address_, uint8_t semi_id_, MdStatus& md_status_, uint32_t usb_timeout_);
		private:
			UsbConnect* usb;
			typedef enum Mode{
				INIT, STATUS, PWM, SPEED, ANGLE, LIM_SW
			}Mode;
	};
};

#pragma once

#include <usb_connect/usb_connect.hpp>

namespace MotorLib{
	class Md{
		public:
			typedef enum LimPort{
				PORT0, PORT1
			}LimPort;

			typedef enum Mode{
				INIT, STATUS, PWM, SPEED, ANGLE, LIM_SW
			}Mode;

			Md(UsbConnect& usb_);
			int sendPwm(uint8_t address_, bool phase_, uint16_t power_, uint32_t usb_timeout_);
			int sendPwm(uint8_t address_, uint8_t semi_id_, bool phase_, uint16_t power_, uint32_t usb_timeout_);
			int sendSpeed(uint8_t address_, bool phase_, uint16_t speed_, uint16_t end_, uint16_t timeout_, uint32_t usb_timeout_);
			int sendSpeed(uint8_t address_, uint8_t semi_id_, bool phase_, uint16_t speed_, uint16_t end_, uint16_t timeout_, uint32_t usb_timeout_);
			int sendAngle(uint8_t address_, uint16_t speed_, int32_t angle_, uint16_t timeout_, uint32_t usb_timeout_);
			int sendAngle(uint8_t address_, uint8_t semi_id_, uint16_t speed_, int32_t angle_, uint16_t timeout_, uint32_t usb_timeout_);
			int sendLimSw(uint8_t address_, bool phase_, uint16_t speed_, LimPort port_, uint16_t timeout_, uint32_t usb_timeout_);
			int sendLimSw(uint8_t address_, uint8_t semi_id_, bool phase_, uint16_t speed_, LimPort port_, uint16_t timeout_, uint32_t usb_timeout_);
			int sendInit(uint8_t address_, bool angle_reset_, uint16_t max_rpm_, uint16_t max_torque_, uint16_t gear_ratio_, uint32_t usb_timeout_);
			int sendInit(uint8_t address_, uint8_t semi_id_, bool angle_reset_, uint16_t max_rpm_, uint16_t max_torque_, uint16_t gear_ratio_, uint32_t usb_timeout_);
			int sendStatus(uint8_t address_, StatusData& md_status_, uint32_t usb_timeout_);
			int sendStatus(uint8_t address_, uint8_t semi_id_, StatusData& md_status_, uint32_t usb_timeout_);
		private:
			UsbConnect* usb;
	};
};

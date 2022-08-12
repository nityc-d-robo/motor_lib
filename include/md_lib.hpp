#pragma once

#include <serial_connect.hpp>

class Md{
	public:
		Md(SerialConnect& serial_);
		void sendPwm(uint16_t address_, bool phase_, uint16_t power_);
		void sendSpeed(uint16_t address_, bool phase_, uint16_t speed_, uint16_t end_);
		void sendAngle(uint16_t address_, bool phase_, uint16_t speed_, uint16_t angle_);
		void sendLimSw(uint16_t address_, bool phase_, uint16_t speed_, bool port_);

	private:
		SerialConnect* serial;
		typedef enum Mode{
			INIT, STATUS, PWM, SPEED, ANGLE, LIM_SW
		}Mode;
};

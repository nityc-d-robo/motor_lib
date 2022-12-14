#pragma once

#include <serial_connect/serial_connect.hpp>

class SMd{
	public:
		SMd(SerialConnect& serial_);
		void sendAngle(uint16_t address_, uint16_t mode_, uint16_t mask_, uint16_t angle1_, uint16_t angle2_);

	private:
		SerialConnect* serial;
		typedef enum Mode{
			INIT, STATUS, SERVO, ESC
		}Mode;
};

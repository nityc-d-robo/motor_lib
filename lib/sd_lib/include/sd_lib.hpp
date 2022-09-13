#pragma once

#include <serial_connect/serial_connect.hpp>

class Sd{
	public:
		Sd(SerialConnect& serial_);
		void sendPowers(uint16_t address_, uint16_t power_1_, uint16_t power_2_);
	private:
		SerialConnect* serial;
};

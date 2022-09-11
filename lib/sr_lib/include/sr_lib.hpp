#pragma once

#include <serial_connect/serial_connect.hpp>

class Sr{
	public:
		Sr(SerialConnect& serial_);
		void sendColors(uint8_t red_, uint8_t green_, uint8_t blue_);
	private:
		SerialConnect* serial;
};

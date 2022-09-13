#include <sd_lib.hpp>

#include <serial_connect/serial_connect.hpp>

Sd::Sd(SerialConnect& serial_){
	serial = &serial_;
}

void Sd::sendPowers(uint16_t address_, uint16_t power_1_, uint16_t power_2_){
	uint8_t send_buf[6] = {0u};

	send_buf[0] = (uint8_t)(address_ + (1 << 4));
	send_buf[2] = (uint8_t)((power_1_ >> 8) & 0xff);
	send_buf[3] = (uint8_t)(power_1_ & 0xff);
	send_buf[4] = (uint8_t)((power_2_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(power_2_ & 0xff);

	serial->writeSerial(send_buf, 6);
}

#include "smd_lib.hpp"

#include <serial_connect/serial_connect.hpp>

SMd::SMd(SerialConnect& serial_){
	serial = &serial_;
}

void SMd::sendAngle(uint16_t address_, uint16_t mode_, uint16_t mask_, uint16_t angle1_, uint16_t angle2_){
	uint8_t send_buf[7] = {0u};

	send_buf[0] = 0x01;
	send_buf[1] = (uint8_t)address_;
	send_buf[2] = (uint8_t)(mode_ << 1) | ((uint8_t)mask_ & 0x01);
	send_buf[3] = (uint8_t)((angle1_ >> 8) & 0xff);
	send_buf[4] = (uint8_t)(angle1_ & 0xff);
	send_buf[5] = (uint8_t)((angle2_ >> 8) & 0xff);
	send_buf[6] = (uint8_t)(angle2_ & 0xff);

	serial->writeSerial(send_buf, 7);
}
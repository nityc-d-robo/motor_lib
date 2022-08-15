#include <md_lib.hpp>

#include <serial_connect/serial_connect.hpp>

Md::Md(SerialConnect& serial_){
	serial = &serial_;
}

void Md::sendPwm(uint16_t address_, bool phase_, uint16_t power_){
	uint8_t send_buf[6] = {0u};

	send_buf[0] = (uint8_t)address_;
	send_buf[1] = (uint8_t)(PWM << 1) | (uint8_t)phase_;
	send_buf[2] = (uint8_t)((power_ >> 8) & 0xff);
	send_buf[3] = (uint8_t)(power_ & 0xff);

	serial->writeSerial(send_buf, 6);
}

void Md::sendSpeed(uint16_t address_, bool phase_, uint16_t speed_, uint16_t end_){
	uint8_t send_buf[6] = {0u};

	send_buf[0] = (uint8_t)address_;
	send_buf[1] = (uint8_t)(SPEED << 1) | (uint8_t)phase_;
	send_buf[2] = (uint8_t)((speed_ >> 8) & 0xff);
	send_buf[3] = (uint8_t)(speed_ & 0xff);
	send_buf[4] = (uint8_t)((end_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(end_ & 0xff);

	serial->writeSerial(send_buf, 6);
}

void Md::sendAngle(uint16_t address_, bool phase_, uint16_t speed_, uint16_t angle_){
	uint8_t send_buf[6] = {0u};

	send_buf[0] = (uint8_t)address_;
	send_buf[1] = (uint8_t)(ANGLE << 1) | (uint8_t)phase_;
	send_buf[2] = (uint8_t)((speed_ >> 8) & 0xff);
	send_buf[3] = (uint8_t)(speed_ & 0xff);
	send_buf[4] = (uint8_t)((angle_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(angle_ & 0xff);

	serial->writeSerial(send_buf, 6);
}

void Md::sendLimSw(uint16_t address_, bool phase_, uint16_t speed_, bool port_){
	uint8_t send_buf[6] = {0u};

	send_buf[0] = (uint8_t)address_;
	send_buf[1] = (uint8_t)(LIM_SW << 1) | (uint8_t)phase_;
	send_buf[2] = (uint8_t)((speed_ >> 8) & 0xff);
	send_buf[3] = (uint8_t)(speed_ & 0xff);
	send_buf[4] = (uint8_t)port_;

	serial->writeSerial(send_buf, 6);
}

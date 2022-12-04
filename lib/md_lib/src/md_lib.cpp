#include <md_lib.hpp>

#include <usb_connect/usb_connect.hpp>

Md::Md(UsbConnect& usb_){
	usb = &usb_;
}

UsbStatus Md::sendPwm(uint8_t address_, bool phase_, uint16_t power_, uint32_t usb_timeout_){
	uint8_t send_buf[10] = {0u};

	send_buf[0] = address_;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)PWM;
	send_buf[3] = (uint8_t)phase_;
	send_buf[4] = (uint8_t)((power_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(power_ & 0xff);

	return usb->writeUsb(send_buf, 10, EndPoint::EP1, usb_timeout_);
}

UsbStatus Md::sendPwm(uint8_t address_, uint8_t semi_id_, bool phase_, uint16_t power_, uint32_t usb_timeout_){
	uint8_t send_buf[10] = {0u};

	send_buf[0] = address_;
	send_buf[1] = semi_id_;
	send_buf[2] = (uint8_t)PWM;
	send_buf[3] = (uint8_t)phase_;
	send_buf[4] = (uint8_t)((power_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(power_ & 0xff);

	return usb->writeUsb(send_buf, 10, EndPoint::EP1, usb_timeout_);
}

UsbStatus Md::sendSpeed(uint8_t address_, bool phase_, uint16_t speed_, uint16_t end_, uint16_t timeout_, uint32_t usb_time_){
	uint8_t send_buf[10] = {0u};

	send_buf[0] = address_;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)SPEED
	send_buf[3] = (uint8_t)phase_;
	send_buf[4] = (uint8_t)((speed_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(speed_ & 0xff);
	send_buf[6] = (uint8_t)((end_ >> 8) & 0xff);
	send_buf[7] = (uint8_t)(end_ & 0xff);
	send_buf[8] = (uint8_t)((timeout_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(timeout_ & 0xff);

	return usb->writeUsb(send_buf, 10, EndPoint::EP1, usb_timeout_);
}

UsbStatus Md::sendSpeed(uint8_t address_, uint8_t semi_id_, bool phase_, uint16_t speed_, uint16_t end_, uint16_t timeout_, uint32_t usb_timeout_){
	uint8_t send_buf[10] = {0u};

	send_buf[0] = address_;
	send_buf[1] = semi_id_;
	send_buf[2] = (uint8_t)SPEED
	send_buf[3] = (uint8_t)phase_;
	send_buf[4] = (uint8_t)((speed_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(speed_ & 0xff);
	send_buf[6] = (uint8_t)((end_ >> 8) & 0xff);
	send_buf[7] = (uint8_t)(end_ & 0xff);
	send_buf[8] = (uint8_t)((timeout_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(timeout_ & 0xff);

	return usb->writeUsb(send_buf, 10, EndPoint::EP1, usb_timeout_);
}

void Md::sendAngle(uint16_t address_, uint16_t speed_, int32_t angle_, uint16_t timeout_, uint32_t usb_timeout_){
	uint8_t send_buf[8] = {0u};

	send_buf[0] = 0x01;
	send_buf[1] = (uint8_t)address_;
	send_buf[2] = (uint8_t)(ANGLE << 1) | (uint8_t)phase_;
	send_buf[3] = (uint8_t)((speed_ >> 8) & 0xff);
	send_buf[4] = (uint8_t)(speed_ & 0xff);
	send_buf[5] = (uint8_t)((angle_ >> 8) & 0xff);
	send_buf[6] = (uint8_t)(angle_ & 0xff);

	serial->writeSerial(send_buf, 7);
}

void Md::sendLimSw(uint16_t address_, bool phase_, uint16_t speed_, bool port_){
	uint8_t send_buf[7] = {0u};

	send_buf[0] = 0x01;
	send_buf[1] = (uint8_t)address_;
	send_buf[2] = (uint8_t)(LIM_SW << 1) | (uint8_t)phase_;
	send_buf[3] = (uint8_t)((speed_ >> 8) & 0xff);
	send_buf[4] = (uint8_t)(speed_ & 0xff);
	send_buf[5] = (uint8_t)port_;

	serial->writeSerial(send_buf, 7);
}

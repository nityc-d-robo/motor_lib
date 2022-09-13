#include <sr_lib.hpp>

#include <serial_connect/serial_connect.hpp>

Sr::Sr(SerialConnect& serial_){
	serial = &serial_;
}

void Sr::sendColors(uint8_t red_, uint8_t green_, uint8_t blue_, float freq_){
	uint8_t send_buf[6] = {0u};

	send_buf[0] = 0x40;
	send_buf[1] = (uint8_t)(freq_ * 4) << 1 | 1u;
	send_buf[2] = green_;
	send_buf[3] = red_;
	send_buf[4] = blue_;

	serial->writeSerial(send_buf, 6);
}

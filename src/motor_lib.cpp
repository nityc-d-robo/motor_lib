#include <motor_lib.hpp>

void motor::stopAll(void){
	uint8_t send_buf[7] = {0u};

	send_buf[0] = 0x01;
	send_buf[1] = 1 << 7;

	if(!serial.isConnect()){
		serial.openSerial();
	}

	serial.writeSerial(send_buf, 7);
}

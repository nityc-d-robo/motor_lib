#include <motor_lib.hpp>

void motor::stopAll(void){
	uint8_t send_buf[6] = {0u};

	send_buf[0] = 1 << 7;

	serial.writeSerial(send_buf, 6);
}

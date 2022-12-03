#include <motor_lib.hpp>
#include <usb_connect/usb_connect.hpp>

void motor::stopAll(void){
	uint8_t send_buf[10] = {0u};

	send_buf[0] = 0xf0;

	if(!serial.isConnect()){
		serial.openSerial();
	}

	while(usb.writeUsb(send_buf, 10, EndPoint::EP1, 300) != 10);
}

#include <usb_connect/usb_connect.hpp>
#include <motor_lib/motor_lib.hpp>
#include <unistd.h>

int main(int argc_, char *argv_[]){
	MotorLib::usb_config.vendor_id = 0x483;
	MotorLib::usb_config.product_id = 0x5740;
	MotorLib::usb_config.b_interface_number = 0;

	MotorLib::usb.setUsb(&MotorLib::usb_config);
	MotorLib::usb.openUsb();

	//red no blinking
	MotorLib::sr.sendColors(255, 0, 0, 0.0f, 1000);

	sleep(3);

	//you can prepare colors that you often use
	MotorLib::Color green_color;
	green_color.red = 0;
	green_color.green = 255;
	green_color.blue = 0;

	//green blink every 0.5 seconds ; 1.0f is brightness (0.0 ~ 1.0)
	MotorLib::sr.sendColors(green_color, 1.0f, 0.5f, 1000);

	sleep(3);

	//blue blink every 0.25 seconds
	MotorLib::sr.sendColors("#0000FF", 0.25f, 1000);

	sleep(3);

	//cut off voltage
	MotorLib::sr.sendStop();

	sleep(2);

	//unblock voltage
	MotorLib::sr.sendStart(1000);

	sleep(2);

	//you can cut off voltage by calling stopALl()
	MotorLib::stopAll();

	MotorLib::usb.closeUsb();
}

#include <usb_connect/usb_connect.hpp>
#include <motor_lib/motor_lib.hpp>
#include <unistd.h>

int main(void){
    MotorLib::usb_config.vendor_id = 0x483;
    MotorLib::usb_config.product_id = 0x5740;
    MotorLib::usb_config.b_interface_number = 0;

    MotorLib::usb.setUsb(&MotorLib::usb_config);
    MotorLib::usb.openUsb();

    MotorLib::md.sendPwm(0x00, NULL, 0, 999, 1000);
    sleep(2);
    MotorLib::stopAll();
    MotorLib::usb.closeUsb();
}
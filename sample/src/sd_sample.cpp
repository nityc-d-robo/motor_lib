#include <usb_connect/usb_connect.hpp>
#include <motor_lib/motor_lib.hpp>
#include <unistd.h>

int main(void){
    MotorLib::usb_config.vendor_id = 0x483;
    MotorLib::usb_config.product_id = 0x5740;
    MotorLib::usb_config.b_interface_number = 0;

    MotorLib::usb.setUsb(&MotorLib::usb_config);
    MotorLib::usb.openUsb();

    MotorLib::sd.sendPowers(0x00, NULL, 999, 999, 5000);
    sleep(2);
    MotorLib::sd.sendPowers(0x00, NULL, 0, 0, 5000);
    MotorLib::stopAll();
    MotorLib::usb.closeUsb();
}
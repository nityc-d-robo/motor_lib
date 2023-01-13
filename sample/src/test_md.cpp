#include <motor_lib/motor_lib.hpp>
#include "unistd.h"
int main(void){
    motor::serial.setSerial("/dev/ttyACM0",B115200);
    motor::serial.openSerial();
    motor::md.sendPwm(0x00, 0, 500);
    usleep(2 * 1000000);
    motor::stopAll();
}
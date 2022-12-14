#include <motor_lib/motor_lib.hpp>
#include "unistd.h"
int main(void){
    motor::serial.setSerial("/dev/ttyACM0",115200);
    motor::serial.openSerial();
    motor::stopAll();
    usleep(50000);
    motor::md.sendPwm(0x01, 0, 300);
    usleep(50000);
}
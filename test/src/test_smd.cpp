#include <motor_lib/motor_lib.hpp>
#include "unistd.h"
int main(void){
    motor::serial.setSerial("/dev/ttyACM0",B115200);
    motor::serial.openSerial();
    motor::smd.sendAngle(0x10, 2, 0, 0);
    usleep(2 * 1000000);
    motor::stopAll();
    usleep(2 * 1000000);
    motor::smd.sendAngle(0x10, 2, 0, 30);
}
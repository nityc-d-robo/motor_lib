#pragma once

#include <md_lib.hpp>

#include <serial_connect/serial_connect.hpp>

SerialConnect serial;

Md md(serial);

void stopAll(void);

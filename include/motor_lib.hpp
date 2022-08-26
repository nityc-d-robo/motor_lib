#pragma once

#ifdef MOTOR_INSTALL
#include <md_lib.hpp>
#else
#include "md_lib/md_lib.hpp"
#endif

#include <serial_connect/serial_connect.hpp>

namespace motor{
	SerialConnect serial(false);

	Md md(serial);

	void stopAll(void);
};

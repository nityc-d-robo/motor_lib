#pragma once

#ifdef MOTOR_INSTALL
#include <md_lib.hpp>
#include <sd_lib.hpp>
#include <sr_lib.hpp>
#else
#include "md_lib/md_lib.hpp"
#include "sd_lib/sd_lib.hpp"
#include "sr_lib/sr_lib.hpp"
#endif

#include <serial_connect/serial_connect.hpp>

namespace motor{
	SerialConnect serial(false);

	Md md(serial);
	Sd sd(serial);
	Sr sr(serial);

	void stopAll(void);
};

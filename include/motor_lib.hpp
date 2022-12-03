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

#include <usb_connect/usb_connect.hpp>

namespace motor{
	UsbConfig usb_config;

	SerialConnect usb;

	Md md(usb);
	Sd sd(usb);
	Sr sr(usb);

	void stopAll(void);
};

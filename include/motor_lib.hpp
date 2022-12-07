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

namespace MotorLib{
	UsbConfig usb_config;

	UsbConnect usb;

	Md md(usb);
	Sd sd(usb);
	Sr sr(usb);

	void stopAll(void);
	int checkFinish(uint8_t address_, uint8_t mode_, uint32_t usb_timeout_);
	int checkFinish(uint8_t address_, uint8_t semi_id_, uint8_t mode_, uint32_t usb_timeout_);
};

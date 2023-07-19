#pragma once

#ifdef MOTOR_INSTALL
#include <md_lib.hpp>
#include <sd_lib.hpp>
#include <smd_lib.hpp>
#include <sr_lib.hpp>
#include <sm_lib.hpp>
#include <common_lib.hpp>
#else
#include "md_lib/md_lib.hpp"
#include "sd_lib/sd_lib.hpp"
#include "smd_lib/smd_lib.hpp"
#include "sr_lib/sr_lib.hpp"
#include "sm_lib/sm_lib.hpp"
#include "common_lib/common_lib.hpp"
#endif

#include <usb_connect/usb_connect.hpp>

namespace MotorLib{
	UsbConfig usb_config;

	UsbConnect usb;

	Md md(usb);
	Sd sd(usb);
	Smd smd(usb);
	Sr sr(usb);
	Sm sm(usb);

	void stopAll(void);
	int checkFinish(uint8_t address_, uint8_t mode_, uint32_t usb_timeout_);
	int checkFinish(uint8_t address_, uint8_t semi_id_, uint8_t mode_, uint32_t usb_timeout_);
};

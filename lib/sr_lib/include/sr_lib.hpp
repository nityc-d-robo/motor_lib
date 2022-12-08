#pragma once

#include <usb_connect/usb_connect.hpp>
#include <common_lib.hpp>

#include <string>

namespace MotorLib{
	class Sr{
		public:
			typedef struct Mode{
				STATUS, STOP, START, COLOR
			}Mode;

			Sr(UsbConnect& usb_);
			void sendStop(void);
			int sendStart(uint32_t usb_timeout_);
			int sendColors(uint8_t red_, uint8_t green_, uint8_t blue_, float freq_, uint32_t usb_timeout_);
			int sendColors(Color color_, float alpha_, float freq_, uint32_t usb_timeout_);
			int sendColors(std::string color_code_, float freq_, uint32_t usb_timeout_);
			int sendStatus(StatusData& sr_status_, uint32_t usb_timeout_);
		private:
			UsbConnect* usb;
	};
};

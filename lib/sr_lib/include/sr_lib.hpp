#pragma once

#include <usb_connect/usb_connect.hpp>
#include <common_lib.hpp>

#include <string>

namespace MotorLib{
	typedef struct Color{
		uint8_t red, green, blue;
	}Color;

	typedef struct SrStatus{
		float firmware;
		bool voltage;
		Color color;
		float freq;
	}SrStatus;

	class Sr{
		public:
			typedef enum Mode{
				STATUS, STOP, START, COLOR
			}Mode;

			Sr(UsbConnect& usb_);
			void sendStop(void);
			int sendStart(uint32_t usb_timeout_);
			int sendColors(uint8_t red_, uint8_t green_, uint8_t blue_, float freq_, uint32_t usb_timeout_);
			int sendColors(Color color_, float alpha_, float freq_, uint32_t usb_timeout_);
			int sendColors(std::string color_code_, float freq_, uint32_t usb_timeout_);
			int sendStatus(SrStatus& sr_status_, uint32_t usb_timeout_);
		private:
			UsbConnect* usb;
	};
};

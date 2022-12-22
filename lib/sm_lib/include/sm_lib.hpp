#pragma once

#include <usb_connect/usb_connect.hpp>
#include <common_lib.hpp>

namespace MotorLib{
	typedef struct SmStatus{
		uint8_t type_num;
		uint8_t datas[4];
	}SmStatus;

	class Sm{
		public:
			typedef enum Mode{
				STATUS, DATA
			}Mode;

			Sm(UsbConnect& usb_);
			int sendDatas(uint8_t address_, uint8_t *write_data_, uint8_t data_size_, uint32_t usb_timeout_);
			int sendStatus(uint8_t address_, SmStatus& sm_status_, uint32_t usb_timeout_);
		private:
			UsbConnect* usb;
	};
};

#include <motor_lib.hpp>
#include <common_lib.hpp>
#include <usb_connect/usb_connect.hpp>

void MotorLib::stopAll(void){
	uint8_t send_buf[10] = {0u};

	send_buf[0] = 0xf0;

	while(usb.writeUsb(send_buf, 10, EndPoint::EP1, 300) != 10);
}

int MotorLib::checkFinish(uint8_t address_, uint8_t mode_, uint32_t usb_timeout_){
	uint8_t read_buf[8] = {0u};
	int return_status = UsbStatus::USB_OTHER_ERROR;

	while(true){
		return_status = usb.readUsb(read_buf, 8, EndPoint::EP1, usb_timeout_);
		
		if(return_status != 8){
			for(auto itr=finish_queue.begin(); itr!=finish_queue.end(); itr++){
				if(*itr.finish_status != FinishStatus::STATUS and *itr.address == address_ and *itr.semi_id == 0u and *itr.mode == mode_){
					FinishStatus status_tmp = *itr.finish_status;
					finish_queue.erase(itr);

					return finish_tmp;
				}
			}

			return return_status;
		}

		if(read_buf[0] != address_ or read_buf[1] != 0u or read_buf[2] == FinishStatus::STATUS){
			FinishData data_tmp;

			data_tmp.address = read_buf[0];
			data_tmp.semi_id = 0u;
			data_tmp.finish_status = read_buf[2];

			if(data_tmp.finish_status == FinishStatus::STATUS){
				data_tmp.firmware = float(read_buf[3]) / 100.0f;
				data_tmp.angle = (int32_t)(read_buf[5] << 8 | read_buf[6]) * ((read_buf[4] == 0u) ? 1 : -1);
				data_tmp.lim_sw1 = bool((read_buf[7] >> 1) & 0x01);
				data_tmp.lim_sw2 = bool(read_buf[7] & 0x01);
			}else{
				data_tmp.mode = read_buf[3];
			}

			finish_queue.push_back(data_tmp);
		}else{
			return read_buf[2];
		}
	}
}

int MotorLib::checkFinish(uint8_t address_, uint8_t semi_id_, uint8_t mode_, uint32_t usb_timeout_){
	uint8_t read_buf[8] = {0u};
	int return_status = UsbStatus::USB_OTHER_ERROR;

	while(true){
		return_status = usb.readUsb(read_buf, 8, EndPoint::EP1, usb_timeout_);
		
		if(return_status != 8){
			for(auto itr=finish_queue.begin(); itr!=finish_queue.end(); itr++){
				if(*itr.finish_status != FinishStatus::STATUS and *itr.address == address_ and *itr.semi_id == semi_id_ and *itr.mode == mode_){
					FinishStatus status_tmp = *itr.finish_status;
					finish_queue.erase(itr);

					return finish_tmp;
				}
			}

			return return_status;
		}

		if(read_buf[0] != address_ or read_buf[1] != semi_id_ or read_buf[2] == FinishStatus::STATUS){
			FinishData data_tmp;

			data_tmp.address = read_buf[0];
			data_tmp.semi_id = read_buf[1];
			data_tmp.finish_status = read_buf[2];

			if(data_tmp.finish_status == FinishStatus::STATUS){
				data_tmp.firmware = float(read_buf[3]) / 100.0f;
				data_tmp.angle = (int32_t)(read_buf[5] << 8 | read_buf[6]) * ((read_buf[4] == 0u) ? 1 : -1);
				data_tmp.lim_sw1 = bool((read_buf[7] >> 1) & 0x01);
				data_tmp.lim_sw2 = bool(read_buf[7] & 0x01);
			}else{
				data_tmp.mode = read_buf[3];
			}

			finish_queue.push_back(data_tmp);
		}else{
			return read_buf[2];
		}
	}
}

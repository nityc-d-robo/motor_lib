#include <motor_lib.hpp>

#include <usb_connect/usb_connect.hpp>
#include <common_lib.hpp>

#include <vector>

void MotorLib::stopAll(void){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = IdType::EMMERGENCY;

	while(usb.writeUsb(send_buf, TX_SIZE, EndPoint::EP1, 300) != TX_SIZE);
}

int MotorLib::checkFinish(uint8_t address_, uint8_t mode_, uint32_t usb_timeout_){
	uint8_t read_buf[RX_SIZE] = {0u};
	int return_status = UsbStatus::USB_OTHER_ERROR;

	while(true){
		return_status = usb.readUsb(read_buf, RX_SIZE, EndPoint::EP1, usb_timeout_);
		
		if(return_status != RX_SIZE){
			for(auto itr=finish_queue.begin(); itr!=finish_queue.end(); itr++){
				if(*itr.finish_status != FinishStatus::STATUS and *itr.address == address_ and *itr.semi_id == 0u){
					if(*itr.datas.finish.mode == mode_){
						FinishStatus status_tmp = *itr.finish_status;
						finish_queue.erase(itr);

						return finish_tmp;
					}
				}
			}

			return return_status;
		}

		if(read_buf[0] != address_ or read_buf[1] != 0u or read_buf[2] == FinishStatus::STATUS){
			StatusData data_tmp;

			data_tmp.address = read_buf[0];
			data_tmp.semi_id = 0u;
			data_tmp.finish_status = read_buf[2];

			if((data_tmp.address & 0xf0) == IdType::SR){
				data_tmp.datas.finish.mode = read_buf[3];
				data_tmp.datas.finish.color.red = read_buf[4];
				data_tmp.datas.finish.color.green = read_buf[5];
				data_tmp.datas.finish.color.blue = read_buf[6];
				data_tmp.datas.finish.freq = float(read_buf[7]) / 4.0f;
			}else if(data_tmp.finish_status == FinishStatus::STATUS){
				data_tmp.datas.status.firmware = float(read_buf[3]) / 100.0f;
				data_tmp.datas.status.angle = (int32_t)(read_buf[5] << 8 | read_buf[6]) * ((read_buf[4] == 0u) ? 1 : -1);
				data_tmp.datas.status.lim_sw0 = bool((read_buf[7] >> 1) & 0x01);
				data_tmp.datas.status.lim_sw1 = bool(read_buf[7] & 0x01);
			}else{
				data_tmp.datas.status.mode = read_buf[3];
			}

			finish_queue.push_back(data_tmp);
		}else{
			return read_buf[2];
		}
	}
}

int MotorLib::checkFinish(uint8_t address_, uint8_t semi_id_, uint8_t mode_, uint32_t usb_timeout_){
	uint8_t read_buf[RX_SIZE] = {0u};
	int return_status = UsbStatus::USB_OTHER_ERROR;

	while(true){
		return_status = usb.readUsb(read_buf, RX_SIZE, EndPoint::EP1, usb_timeout_);
		
		if(return_status != RX_SIZE){
			for(auto itr=finish_queue.begin(); itr!=finish_queue.end(); itr++){
				if(*itr.finish_status != FinishStatus::STATUS and *itr.address == address_ and *itr.semi_id == (semi_id_ | IdType::SM)){
					if(*itr.datas.finish.mode == mode_){
						FinishStatus status_tmp = *itr.finish_status;
						finish_queue.erase(itr);

						return finish_tmp;
					}
				}
			}

			return return_status;
		}

		if(read_buf[0] != address_ or read_buf[1] != (semi_id_ | IdType::SM) or read_buf[2] == FinishStatus::STATUS){
			StatusData data_tmp;

			data_tmp.address = read_buf[0];
			data_tmp.semi_id = read_buf[1];
			data_tmp.finish_status = read_buf[2];

			if((data_tmp.address & 0xf0) == IdType::SR){
				data_tmp.datas.finish.mode = read_buf[3];
				data_tmp.datas.finish.color.red = read_buf[4];
				data_tmp.datas.finish.color.green = read_buf[5];
				data_tmp.datas.finish.color.blue = read_buf[6];
				data_tmp.datas.finish.freq = float(read_buf[7]) / 4.0f;
			}else if(data_tmp.finish_status == FinishStatus::STATUS){
				data_tmp.datas.status.firmware = float(read_buf[3]) / 100.0f;
				data_tmp.datas.status.angle = (int32_t)(read_buf[5] << 8 | read_buf[6]) * ((read_buf[4] == 0u) ? 1 : -1);
				data_tmp.datas.status.lim_sw0 = bool((read_buf[7] >> 1) & 0x01);
				data_tmp.datas.status.lim_sw1 = bool(read_buf[7] & 0x01);
			}else{
				data_tmp.datas.status.mode = read_buf[3];
			}

			finish_queue.push_back(data_tmp);
		}else{
			return read_buf[2];
		}
	}
}

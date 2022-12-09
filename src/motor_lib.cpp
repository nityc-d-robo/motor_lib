#include <motor_lib.hpp>

#include <usb_connect/usb_connect.hpp>
#include <common_lib.hpp>

#include <vector>
#include <array>

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
						FinishStatus status_tmp = *itr[2];
						finish_queue.erase(itr);

						return finish_tmp;
					}
				}
			}

			return return_status;
		}

		if(read_buf[0] != address_ or read_buf[1] != 0u or read_buf[2] == FinishStatus::STATUS){
			std::array<uint8_t, RX_SIZE> data_tmp;

			memcpy(&data_tmp, read_buf, RX_SIZE);

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
					if(*itr.datas.mode == mode_){
						FinishStatus status_tmp = *itr[2];
						finish_queue.erase(itr);

						return finish_tmp;
					}
				}
			}

			return return_status;
		}

		if(read_buf[0] != address_ or read_buf[1] != (semi_id_ | IdType::SM) or read_buf[2] == FinishStatus::STATUS){
			std::array<uint8_t, RX_SIZE> data_tmp;

			memcpy(&data_tmp, read_buf, RX_SIZE);

			finish_queue.push_back(data_tmp);
		}else{
			return read_buf[2];
		}
	}
}

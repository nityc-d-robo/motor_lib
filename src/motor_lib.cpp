#include <motor_lib.hpp>

#include <usb_connect/usb_connect.hpp>
#include <common_lib.hpp>

#include <cstring>
#include <vector>
#include <array>

void MotorLib::stopAll(void){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = IdType::EMMERGENCY;
	send_buf[1] = IdType::EMMERGENCY;

	while(usb.writeUsb(send_buf, TX_SIZE, EndPoint::EP1, 300) != TX_SIZE);
}

int MotorLib::checkFinish(uint8_t address_, uint8_t mode_, uint32_t usb_timeout_){
	uint8_t read_buf[RX_SIZE] = {0u};
	int return_status = UsbStatus::USB_OTHER_ERROR;

	while(true){
		return_status = usb.readUsb(read_buf, RX_SIZE, EndPoint::EP1, usb_timeout_);
		
		if(return_status != RX_SIZE){
			for(auto itr=finish_queue.begin(); itr!=finish_queue.end(); itr++){
				if((*itr)[Header::MODE] != FinishStatus::STATUS and (*itr)[Header::ADDRESS] == address_ and (*itr)[Header::SEMI_ID] == 0u){
					if((*itr)[3] == mode_){
						int status_tmp = (*itr)[Header::MODE];
						finish_queue.erase(itr);

						return status_tmp;
					}
				}
			}

			return return_status;
		}

		if(read_buf[Header::ADDRESS] != address_ or read_buf[Header::SEMI_ID] != 0u or read_buf[Header::MODE] == FinishStatus::STATUS or read_buf[3] != mode_){
			std::array<uint8_t, RX_SIZE> data_tmp;

			memcpy(&data_tmp, read_buf, RX_SIZE);

			finish_queue.push_back(data_tmp);
		}else{
			return read_buf[Header::MODE];
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
				if((*itr)[Header::MODE] != FinishStatus::STATUS and (*itr)[Header::ADDRESS] == address_ and (*itr)[Header::SEMI_ID] == (semi_id_ | IdType::SM)){
					if((*itr)[3] == mode_){
						int status_tmp = (*itr)[Header::MODE];
						finish_queue.erase(itr);

						return status_tmp;
					}
				}
			}

			return return_status;
		}

		if(read_buf[Header::ADDRESS] != address_ or read_buf[Header::SEMI_ID] != (semi_id_ | IdType::SM) or read_buf[Header::MODE] == FinishStatus::STATUS or read_buf[3] != mode_){
			std::array<uint8_t, RX_SIZE> data_tmp;

			memcpy(&data_tmp, read_buf, RX_SIZE);

			finish_queue.push_back(data_tmp);
		}else{
			return read_buf[Header::MODE];
		}
	}
}

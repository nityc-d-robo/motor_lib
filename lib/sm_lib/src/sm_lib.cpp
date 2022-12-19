#include <sm_lib.hpp>

#include <usb_connect/usb_connect.hpp>

#include <cstring>
#include <string>
#include <vector>
#include <array>

MotorLib::Sm::Sm(UsbConnect& usb_){
	usb = &usb_;
}

int MotorLib::Sm::sendDatas(uint8_t address_, uint8_t* write_data_, uint8_t data_size_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	if(data_size_ > 7){
		errorMotor("Too long message.");

		return UsbStatus::USB_OTHER_ERROR;
	}

	send_buf[0] = address_ | IdType::SM;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)this->Mode::DATA;

	memcpy(send_buf + 3, write_data_, data_size_);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Sm::sendStatus(uint8_t address_, SmStatus& sm_status_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};
	uint8_t read_buf[RX_SIZE] = {0u};
	int return_status = UsbStatus::USB_OTHER_ERROR;

	send_buf[0] = address | IdType::SM;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)this->Mode::STATUS;
	
	return_status = usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);

	if(return_status != TX_SIZE){
		return return_status;
	}

	while(true){
		return_status = usb->readUsb(read_buf, RX_SIZE, EndPOint::EP1, usb_timeout_);

		if(return_status != RX_SIZE){
			for(auto ifr=finish_queue.begin(); itr!=finish_queue.end(); itr++){
				if(*itr[2] == FinishStatus::STATUS and *itr[0] == (address_ | IdType::SM) and *itr[1] == 0u){
					sm_status_.type_num = *itr[3];
					
					memcpy(sm_status_.datas, &(*itr[4]), 4);

					finish_queue.erase(itr);

					return RX_SIZE;
				}
			}

			return return_status;
		}

		if(read_buf[0] != (address_ | IdType::SM) or read_buf[1] != 0u or read_buf[2] != FinishStatus::STATUS){
			std::array<uint8_t, RX_SIZE> data_tmp;

			memcpy(&data_tmp, read_buf, RX_SIZE);

			finish_queue.push_back(data_tmp);
		}else{
			sm_status_.type_num = read_buf[3];

			memcpy(sm_status_.datas, read_buf+4, 4);

			return return_status;
		}
	}
}

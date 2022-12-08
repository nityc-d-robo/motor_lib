#include <sd_lib.hpp>

#include <usb_connect/usb_connect.hpp>
#include <common_lib.hpp>

#include <vector>

MotorLib::Sd::Sd(UsbConnect& usb_){
	 usb = &usb_;
}

int MotorLib::Sd::sendPowers(uint8_t address_, uint16_t power1_, uint16_t power2_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_ | IdType::SD;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)Mode::POWER;
	send_buf[4] = (uint8_t)((power1_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(power1_ & 0xff);
	send_buf[6] = (uint8_t)((power2_ >> 8) & 0xff);
	send_buf[7] = (uint8_t)(power2_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Sd::sendPowers(uint8_t address_, uint8_t semi_id_, uint16_t power1_, uint16_t power2_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_ | IdType::SD;
	send_buf[1] = semi_id_ | IdType::SM;
	send_buf[2] = (uint8_t)Mode::POWER;
	send_buf[4] = (uint8_t)((power1_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(power1_ & 0xff);
	send_buf[6] = (uint8_t)((power2_ >> 8) & 0xff);
	send_buf[7] = (uint8_t)(power2_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Sd::sendLimSw(uint8_t address_, OutPort out_port_, LimPort lim_port_, uint16_t first_power_, uint16_t next_power_, uint16_t timeout_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_ | IdType::SD;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)Mode::LIM_SW;
	send_buf[3] = (uint8_t)(out_port << 1 | lim_port_);
	send_buf[4] = (uint8_t)((first_power_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(first_power_ & 0xff);
	send_buf[6] = (uint8_t)((next_power_ >> 8) & 0xff);
	send_buf[7] = (uint8_t)(next_power_ & 0xff);
	send_buf[8] = (uint8_t)((timeout_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(timeout_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Sd::sendLimSw(uint8_t address_, uint8_t semi_id_, OutPort out_port_, LimPort lim_port_, uint16_t first_power_, uint16_t next_power_, uint16_t timeout_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_ | IdType::SD;
	send_buf[1] = semi_id_ | IdType::SM;
	send_buf[2] = (uint8_t)Mode::LIM_SW;
	send_buf[3] = (uint8_t)(out_port << 1 | lim_port_);
	send_buf[4] = (uint8_t)((first_power_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(first_power_ & 0xff);
	send_buf[6] = (uint8_t)((next_power_ >> 8) & 0xff);
	send_buf[7] = (uint8_t)(next_power_ & 0xff);
	send_buf[8] = (uint8_t)((timeout_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(timeout_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Sd::sendStatus(uint8_t address_, StatusData& sd_status_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};
	uint8_t read_buf[RX_SIZE] = {0u};
	int return_status = UsbStatus::USB_OTHER_ERROR;

	send_buf[0] = address_ | IdType::SD;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)Mode::STATUS;

	return_status = usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);

	if(return_status != TX_SIZE){
		return return_status;
	}

	while(true){
		return_status = usb->readUsb(read_buf, RX_SIZE, EndPoint::EP1, usb_timeout_);

		if(return_status != RX_SIZE){
			for(auto itr=finish_queue,begin(); itr!=finish_queue.end(); itr++){
				if(*itr.finish_status == FinishStatus::STATUS and *itr.address == (address_ | IdType::SD) and *itr.semi_id == 0){
					sd_status_ = *itr;

					finish_queue.erase(itr);

					return RX_SIZE;
				}
			}

			return return_status;
		}

		if(read_buf[0] != (address_ | IdType::SD) or read_buf[1] != 0u or read_buf[2] != FinishStatus::STATUS){
			StatusData status_tmp;

			status_tmp.address = read_buf[0];
			status_tmp.semi_id = read_buf[1];
			status_tmp.finish_status = read_buf[2];

			if((status_tmp.address & 0xf0) == IdType::SR){
				status_tmp.datas.finish.mode = read_buf[3];
				status_tmp.datas.finish.color.red = read_buf[4];
				status_tmp.datas.finish.color.green = read_buf[5];
				status_tmp.datas.finish.color.blue = read_buf[6];
				status_tmp.datas.finish.freq = float(read_buf[7]) / 4.0f;
			}else if(status_tmp.finish_status == FinishStatus::STATUS){
				status_tmp.datas.status.firmware = float(read_buf[3]) / 100.0f;
				status_tmp.datas.status.lim_sw0 = bool((read_buf[7] >> 1) & 0x01);
				status_tmp.datas.status.lim_sw1 = bool(read_buf[7] & 0x01);
			}else{
				status_tmp.datas.status.mode = read_buf[3];
			}

			finish_queue.push_back(status_tmp);
		}else{
			sd_status_.address = read_buf[0];
			sd_status_.semi_id = 0u;
			sd_status_.datas.status.firmware = float(read_buf[3]) / 100.0f;
			sd_status_.datas.status.lim_sw0 = bool((read_buf[7] >> 1) & 0x01);
			sd_status_.datas.status.lim_sw1 = bool(read_buf[7] & 0x01);

			return return_status;
		}
	}
}


int MotorLib::Sd::sendStatus(uint8_t address_, uint8_t semi_id_, StatusData& sd_status_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};
	uint8_t read_buf[RX_SIZE] = {0u};
	int return_status = UsbStatus::USB_OTHER_ERROR;

	send_buf[0] = address_ | IdType::SD;
	send_buf[1] = semi_id_ | IdType::SM;
	send_buf[2] = (uint8_t)Mode::STATUS;

	return_status = usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);

	if(return_status != TX_SIZE){
		return return_status;
	}

	while(true){
		return_status = usb->readUsb(read_buf, RX_SIZE, EndPoint::EP1, usb_timeout_);

		if(return_status != RX_SIZE){
			for(auto itr=finish_queue,begin(); itr!=finish_queue.end(); itr++){
				if(*itr.finish_status == FinishStatus::STATUS and *itr.address == (address_ | IdType::SD) and *itr.semi_id == (semi_id_ | IdType::SM)){
					sd_status_ = *itr;

					finish_queue.erase(itr);

					return RX_SIZE;
				}
			}

			return return_status;
		}

		if(read_buf[0] != (address_ | IdType::SD) or read_buf[1] != (semi_id_ | IdType::SM) or read_buf[2] != FinishStatus::STATUS){
			StatusData status_tmp;

			status_tmp.address = read_buf[0];
			status_tmp.semi_id = read_buf[1];
			status_tmp.finish_status = read_buf[2];

			if((status_tmp.address & 0xf0) == IdType::SR){
				status_tmp.datas.finish.mode = read_buf[3];
				status_tmp.datas.finish.color.red = read_buf[4];
				status_tmp.datas.finish.color.green = read_buf[5];
				status_tmp.datas.finish.color.blue = read_buf[6];
				status_tmp.datas.finish.freq = float(read_buf[7]) / 4.0f;
			}else if(status_tmp.finish_status == FinishStatus::STATUS){
				status_tmp.datas.status.firmware = float(read_buf[3]) / 100.0f;
				status_tmp.datas.status.lim_sw0 = bool((read_buf[7] >> 1) & 0x01);
				status_tmp.datas.status.lim_sw1 = bool(read_buf[7] & 0x01);
			}else{
				status_tmp.datas.finish.mode = read_buf[3];
			}

			finish_queue.push_back(status_tmp);
		}else{
			sd_status_.address = read_buf[0];
			sd_status_.semi_id = read_buf[1];
			sd_status_.datas.status.firmware = float(read_buf[3]) / 100.0f;
			sd_status_.datas.status.lim_sw0 = bool((read_buf[7] >> 1) & 0x01);
			sd_status_.datas.status.lim_sw1 = bool(read_buf[7] & 0x01);

			return return_status;
		}
	}
}

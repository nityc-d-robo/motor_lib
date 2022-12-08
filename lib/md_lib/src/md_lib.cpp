#include <md_lib.hpp>

#include <usb_connect/usb_connect.hpp>
#include <common_lib.hpp>

#include <vector>

MotorLib::Md::Md(UsbConnect& usb_){
	usb = &usb_;
}

int MotorLib::Md::sendPwm(uint8_t address_, bool phase_, uint16_t power_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)PWM;
	send_buf[3] = (uint8_t)phase_;
	send_buf[4] = (uint8_t)((power_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(power_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Md::sendPwm(uint8_t address_, uint8_t semi_id_, bool phase_, uint16_t power_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_;
	send_buf[1] = semi_id_ | IdType::SM;
	send_buf[2] = (uint8_t)PWM;
	send_buf[3] = (uint8_t)phase_;
	send_buf[4] = (uint8_t)((power_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(power_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Md::sendSpeed(uint8_t address_, bool phase_, uint16_t speed_, uint16_t end_, uint16_t timeout_, uint32_t usb_time_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)SPEED
	send_buf[3] = (uint8_t)phase_;
	send_buf[4] = (uint8_t)((speed_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(speed_ & 0xff);
	send_buf[6] = (uint8_t)((end_ >> 8) & 0xff);
	send_buf[7] = (uint8_t)(end_ & 0xff);
	send_buf[8] = (uint8_t)((timeout_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(timeout_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLub::Md::sendSpeed(uint8_t address_, uint8_t semi_id_, bool phase_, uint16_t speed_, uint16_t end_, uint16_t timeout_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_;
	send_buf[1] = semi_id_ | IdType::SM;
	send_buf[2] = (uint8_t)SPEED
	send_buf[3] = (uint8_t)phase_;
	send_buf[4] = (uint8_t)((speed_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(speed_ & 0xff);
	send_buf[6] = (uint8_t)((end_ >> 8) & 0xff);
	send_buf[7] = (uint8_t)(end_ & 0xff);
	send_buf[8] = (uint8_t)((timeout_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(timeout_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Md::sendAngle(uint8_t address_, uint16_t speed_, int32_t angle_, uint16_t timeout_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_;
	send_buf[1] = 0;
	send_buf[2] = (uint8_t)ANGLE
	send_buf[3] = (angle_ >= 0) ? 1u : 0u;
	send_buf[4] = (uint8_t)((speed_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(speed_ & 0xff);
	send_buf[6] = (uint8_t)((std::abs(angle_) >> 8) & 0xff);
	send_buf[7] = (uint8_t)(std::abs(angle_) & 0xff);
	send_buf[8] = (uint8_t)((timeout_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(timeout_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Md::sendAngle(uint8_t address_, uint8_t semi_id_, uint16_t speed_, int32_t angle_, uint16_t timeout_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_;
	send_buf[1] = semi_id_ | IdType::SM;
	send_buf[2] = (uint8_t)ANGLE;
	send_buf[3] = (angle_ >= 0) ? 1u : 0u;
	send_buf[4] = (uint8_t)((speed_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(speed_ & 0xff);
	send_buf[6] = (uint8_t)((std::abs(angle_) >> 8) & 0xff);
	send_buf[7] = (uint8_t)(std::abs(angle_) & 0xff);
	send_buf[8] = (uint8_t)((timeout_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(timeout_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Md::sendLimSw(uint8_t address_, bool phase_, uint16_t speed_, LimPort port_, uint16_t timeout_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)LIM_SW; 
	send_buf[3] = (uint8_t)phase_;
	send_buf[4] = (uint8_t)((speed_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(speed_ & 0xff);
	send_buf[6] = (uint8_t)port_;
	send_buf[7] = 0u;
	send_buf[8] = (uint8_t)((timeout_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(timeout_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Md::sendLimSw(uint8_t address_, uint8_t semi_id_, bool phase_, uint16_t speed_, LimPort port_, uint16_t timeout_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_;
	send_buf[1] = semi_id_ | IdType::SM;
	send_buf[2] = (uint8_t)LIM_SW; 
	send_buf[3] = (uint8_t)phase_;
	send_buf[4] = (uint8_t)((speed_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(speed_ & 0xff);
	send_buf[6] = (uint8_t)port_;
	send_buf[7] = 0u;
	send_buf[8] = (uint8_t)((timeout_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(timeout_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Md::sendInit(uint8_t address_, bool angle_reset_, uint16_t max_rpm_, uint16_t max_torque_, uint16_t gear_ratio_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)Mode::INIT;
	send_buf[3] = (uint8_t)angle_reset_;
	send_buf[4] = (uint8_t)((max_rpm_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(max_rpm_ & 0xff);
	send_buf[6] = (uint8_t)((max_torque_ >> 8) & 0xff);
	send_buf[7] = (uint8_t)(max_torque & 0xff);
	send_buf[8] = (uint8_t)((gear_ratio_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(gear_ratio_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Md::sendInit(uint8_t address_, uint8_t semi_id_, bool angle_reset_, uint16_t max_rpm_, uint16_t max_torque_, uint16_t gear_ratio_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_;
	send_buf[1] = semi_id_ | IdType::SM;
	send_buf[2] = (uint8_t)Mode::INIT;
	send_buf[3] = (uint8_t)angle_reset_;
	send_buf[4] = (uint8_t)((max_rpm_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(max_rpm_ & 0xff);
	send_buf[6] = (uint8_t)((max_torque_ >> 8) & 0xff);
	send_buf[7] = (uint8_t)(max_torque & 0xff);
	send_buf[8] = (uint8_t)((gear_ratio_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(gear_ratio_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Md::sendStatus(uint8_t address_, StatusData& md_status_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};
	uint8_t read_buf[RX_SIZE] = {0u};
	int return_status = UsbStatus::USB_OTHER_ERROR;

	send_buf[0] = addreess_;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)Mode::STATUS;

	return_status = usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);

	if(return_status != TX_SIZE){
		return return_status;
	}

	while(true){
		return_status = usb->readUsb(read_buf, RX_SIZE, EndPoint::EP1, usb_timeout_);

		if(return_status != RX_SIZE){
			for(auto itr=finish_queue.begin(); itr!=finish_queue.end(); itr++){
				if(*itr.finish_status == FinishStatus::STATUS and *itr.adderss == address_ and *itr.semi_id = 0u){
					md_status_ = *itr;

					finish_queue.erase(itr);

					return 8;
				}
			}

			return return_status;
		}

		if(read_buf[0] != address_ or read_buf[1] != 0u or read_buf[2] != FinishStatus::STATUS){
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
				status_tmp.datas.status.angle = (int32_t)(read_buf[5] << 8 | read_buf[6]) * ((read_buf[4] == 0u) ? 1 : -1);
				status_tmp.datas.status.lim_sw1 = bool((read_buf[7] >> 1) & 0x01);
				status_tmp.datas.status.lim_sw2 = bool(read_buf[7] & 0x01);
			}else{
				status_tmp.datas.finish.mode = read_buf[3];
			}

			finish_queue.push_back(status_tmp);
		}else{
			md_status_.address = read_buf[0];
			md_status_.semi_id = 0u;
			md_status_.datas.status.firmware = float(read_buf[3]) / 100.0f;
			md_status_.datas.status.angle = (int32_t)(read_buf[5] << 8 | read_buf[6]) * ((read_buf[4] == 0u) ? 1 : -1);
			md_status_.datas.status.lim_sw0 = bool((read_buf[7] >> 1) & 0x01);
			md_status_.datas.status.lim_sw1 = bool(read_buf[7] & 0x01);

			return return_status;
		}
	}
}

int MotorLib::Md::sendStatus(uint8_t address_, uint8_t semi_id_, StatusData& md_status_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};
	uint8_t read_buf[RX_SIZE] = {0u};
	int return_status = UsbStatus::USB_OTHER_ERROR;

	send_buf[0] = addreess_;
	send_buf[1] = semi_id_ | IdType::SM;
	send_buf[2] = (uint8_t)Mode::STATUS;

	return_status = usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);

	if(return_status != TX_SIZE){
		return return_status;
	}

	while(true){
		return_status = usb->readUsb(read_buf, RX_SIZE, EndPoint::EP1, usb_timeout_);

		if(return_status != RX_SIZE){
			for(auto itr=finish_queue.begin(); itr!=finish_queue.end(); itr++){
				if(*itr.finish_status == FinishStatus::STATUS and *itr.adderss == address_ and *itr.semi_id == (semi_id_ | IdType::SM)){
					md_status_ = *itr;

					finish_queue.erase(itr);

					return RX_SIZE;
				}
			}

			return return_status;
		}

		if(read_buf[0] != address_ or read_buf[1] != (semi_id_ | IdType::SM) or read_buf[2] != FinishStatus::STATUS){
			StatusData status_tmp;

			status_tmp.address = read_buf[0];
			status_tmp.semi_id = read_buf[1];
			status_tmp.finish_status = read_buf[2];

			if((status_tmp.address & 0xf0) == IdType::SR){
				status_tmp.datas.finish.mode = read_buf[3];
				status_tmp.datas.finish.color.red = read_buf[4];
				status_tmp.datas.finish.color.green = read_buf[5];
				status_tmp.datas.finish.color.blue = read_buf[6];
				status_tmp.datas.finish.freq = float(raed_buf[7]) / 4.0f;
			}else if(status_tmp.finish_status == FinishStatus::STATUS){
				status_tmp.datas.status.firmware = float(read_buf[3]) / 100.0f;
				statis_tmp.datas.status.angle = (int32_t)(read_buf[5] << 8 | read_buf[6]) * ((read_buf[4] == 0u) ? 1 : -1);
				status_tmp.datas.status.lim_sw0 = bool((read_buf[7] >> 1) & 0x01);
				status_tmp.datas.status.lim_sw1 = bool(read_buf[7] & 0x01);
			}else{
				status_tmp.datas.finish.mode = read_buf[3];
			}

			finish_queue.push_back(status_tmp);
		}else{
			md_status_.address = read_buf[0];
			md_status_.semi_id = read_buf[1];
			md_status_.firmware = float(read_buf[3]) / 100.0f;
			md_status_.datas.status.angle = (int32_t)(read_buf[5] << 8 | read_buf[6]) * ((read_buf[4] == 0u) ? 1 : -1);
			md_status_.datas.status.lim_sw0 = bool((read_buf[7] >> 1) & 0x01);
			md_status_.datas.status.lim_sw1 = bool(read_buf[7] & 0x01);

			return return_status;
		}
	}
}

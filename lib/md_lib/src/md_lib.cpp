#include <md_lib.hpp>

#include <usb_connect/usb_connect.hpp>
#include <common_lib.hpp>

#include <vector>
#include <array>
#include <cstring>

MotorLib::Md::Md(UsbConnect& usb_){
	usb = &usb_;
}

int MotorLib::Md::sendPwm(uint8_t address_, bool phase_, uint16_t power_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_;
	send_buf[1] = (uint8_t)IdType::MASTER;
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

int MotorLib::Md::sendSpeed(uint8_t address_, bool phase_, uint16_t speed_, uint16_t end_, uint16_t timeout_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_;
	send_buf[1] = (uint8_t)IdType::MASTER;
	send_buf[2] = (uint8_t)SPEED;
	send_buf[3] = (uint8_t)phase_;
	send_buf[4] = (uint8_t)((speed_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(speed_ & 0xff);
	send_buf[6] = (uint8_t)((end_ >> 8) & 0xff);
	send_buf[7] = (uint8_t)(end_ & 0xff);
	send_buf[8] = (uint8_t)((timeout_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(timeout_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Md::sendSpeed(uint8_t address_, uint8_t semi_id_, bool phase_, uint16_t speed_, uint16_t end_, uint16_t timeout_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_;
	send_buf[1] = semi_id_ | IdType::SM;
	send_buf[2] = (uint8_t)SPEED;
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
	send_buf[1] = (uint8_t)IdType::MASTER;
	send_buf[2] = (uint8_t)ANGLE;
	send_buf[3] = (angle_ >= 0) ? 0u : 1u;
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
	send_buf[3] = (angle_ >= 0) ? 0u : 1u;
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
	send_buf[1] = (uint8_t)IdType::MASTER;
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
	send_buf[1] = (uint8_t)IdType::MASTER;
	send_buf[2] = (uint8_t)Mode::INIT;
	send_buf[3] = (uint8_t)angle_reset_;
	send_buf[4] = (uint8_t)((max_rpm_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(max_rpm_ & 0xff);
	send_buf[6] = (uint8_t)((max_torque_ >> 8) & 0xff);
	send_buf[7] = (uint8_t)(max_torque_ & 0xff);
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
	send_buf[7] = (uint8_t)(max_torque_ & 0xff);
	send_buf[8] = (uint8_t)((gear_ratio_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(gear_ratio_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Md::sendStatus(uint8_t address_, MdStatus& md_status_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};
	uint8_t read_buf[RX_SIZE] = {0u};
	int return_status = UsbStatus::USB_OTHER_ERROR;

	send_buf[0] = address_;
	send_buf[1] = IdType::MASTER;
	send_buf[2] = (uint8_t)Mode::STATUS;

	return_status = usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);

	if(return_status != TX_SIZE){
		return return_status;
	}

	while(true){
		return_status = usb->readUsb(read_buf, RX_SIZE, EndPoint::EP1, usb_timeout_);

		if(return_status != RX_SIZE){
			for(auto itr=finish_queue.begin(); itr!=finish_queue.end(); itr++){
				if((*itr)[Header::MODE] == FinishStatus::STATUS and (*itr)[Header::ADDRESS] == address_ and (*itr)[Header::SEMI_ID] == IdType::MASTER){
					md_status_.firmware = float((*itr)[3]) / 10.0f;
					md_status_.angle = (int32_t)(((*itr)[5] << 8) | (*itr)[6]) * ((*itr)[4] == 0) ? 1 : -1;
					md_status_.lim_sw0 = bool(((*itr)[7] >> 1) & 0x01);
					md_status_.lim_sw1 = bool((*itr)[7] & 0x01);

					finish_queue.erase(itr);

					return 8;
				}
			}

			return return_status;
		}

		if(read_buf[Header::ADDRESS] != address_ or read_buf[Header::SEMI_ID] != IdType::MASTER or read_buf[Header::MODE] != FinishStatus::STATUS){
			std::array<uint8_t, RX_SIZE> status_tmp;

			memcpy(&status_tmp, read_buf, RX_SIZE);

			finish_queue.push_back(status_tmp);
		}else{
			md_status_.firmware = float(read_buf[3]) / 10.0f;
			md_status_.angle = (int32_t)(read_buf[5] << 8 | read_buf[6]) * ((read_buf[4] == 0u) ? 1 : -1);
			md_status_.lim_sw0 = bool((read_buf[7] >> 1) & 0x01);
			md_status_.lim_sw1 = bool(read_buf[7] & 0x01);

			return return_status;
		}
	}
}

int MotorLib::Md::sendStatus(uint8_t address_, uint8_t semi_id_, MdStatus& md_status_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};
	uint8_t read_buf[RX_SIZE] = {0u};
	int return_status = UsbStatus::USB_OTHER_ERROR;

	send_buf[0] = address_;
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
				if((*itr)[Header::MODE] == FinishStatus::STATUS and (*itr)[Header::ADDRESS] == address_ and (*itr)[Header::SEMI_ID] == (semi_id_ | IdType::SM)){
					md_status_.firmware = float((*itr)[3]) / 10.0f;
					md_status_.angle = (int32_t)(((*itr)[5] << 8) | (*itr)[6]) * ((*itr)[4] & 0x01 == 0) ? 1 : -1;
					md_status_.lim_sw0 = bool(((*itr)[7] >> 1) & 0x01);
					md_status_.lim_sw1 = bool((*itr)[7] & 0x01);

					finish_queue.erase(itr);

					return RX_SIZE;
				}
			}

			return return_status;
		}

		if(read_buf[Header::ADDRESS] != address_ or read_buf[Header::SEMI_ID] != (semi_id_ | IdType::SM) or read_buf[Header::MODE] != FinishStatus::STATUS){
			std::array<uint8_t, RX_SIZE> data_tmp;

			memcpy(&data_tmp, read_buf, RX_SIZE);

			finish_queue.push_back(data_tmp);
		}else{
			md_status_.firmware = float(read_buf[3]) / 10.0f;
			md_status_.angle = (int32_t)(read_buf[5] << 8 | read_buf[6]) * ((read_buf[4] == 0u) ? 1 : -1);
			md_status_.lim_sw0 = bool((read_buf[7] >> 1) & 0x01);
			md_status_.lim_sw1 = bool(read_buf[7] & 0x01);

			return return_status;
		}
	}
}

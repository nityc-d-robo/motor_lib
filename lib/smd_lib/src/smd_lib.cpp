#include <smd_lib.hpp>
#include <common_lib.hpp>

#include <usb_connect/usb_connect.hpp>

#include <cstring>
#include <array>
#include <vector>

MotorLib::Smd::Smd(UsbConnect& usb_){
	usb = &usb_;
}

int MotorLib::Smd::sendAngle(uint8_t address_, Port port_, uint8_t angle_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_ | IdType::SMD;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)this->Mode::ANGLE;
	send_buf[3] = (uint8_t)port_;
	send_buf[4] = angle_;

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Smd::sendAngle(uint8_t address_, uint8_t semi_id_, Port port_, uint8_t angle_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_ | IdType::SMD;
	send_buf[1] = semi_id_ | IdType::SM;
	send_buf[2] = (uint8_t)this->Mode::ANGLE;
	send_buf[3] = (uint8_t)port_;
	send_buf[4] = angle_;

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Smd::sendAngleBoth(uint8_t address_, uint8_t angle0_, uint8_t angle1_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_ | IdType::SMD;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)this->Mode::ANGLE_BOTH;
	send_buf[3] = 0u;
	send_buf[4] = angle0_;
	send_buf[5] = angle1_;

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Smd::sendAngleBoth(uint8_t address_, uint8_t semi_id_, uint8_t angle0_, uint8_t angle1_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_ | IdType::SMD;
	send_buf[1] = semi_id_ | IdType::SMD;
	send_buf[2] = (uint8_t)this->Mode::ANGLE_BOTH;
	send_buf[3] = 0u;
	send_buf[4] = angle0_;
	send_buf[5] = angle1_;

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Smd::sendStatus(uint8_t address_, SmdStatus& smd_status_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};
	uint8_t read_buf[RX_SIZE] = {0u};
	int return_status;

	send_buf[0] = address_ | IdType::SMD;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)this->Mode::STATUS;

	return_status = usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);

	if(return_status != TX_SIZE){
		return return_status;
	}

	while(true){
		return_status = usb->readUsb(read_buf, RX_SIZE, EndPoint::EP1, usb_timeout_);

		if(return_status != RX_SIZE){
			for(auto itr=finish_queue.begin(); itr!=finish_queue.end(); itr++){
				if((*itr)[Header::MODE] == FinishStatus::STATUS and (*itr)[Header::ADDRESS] == (address_ | IdType::SMD) and (*itr)[Header::SEMI_ID] == 0u){
					smd_status_.firmware = float((*itr)[3]) / 10.0f;
					smd_status_.angle0 = (*itr)[4];
					smd_status_.angle1 = (*itr)[5];

					finish_queue.erase(itr);

					return RX_SIZE;
				}
			}

			return return_status;
		}

		if(read_buf[Header::ADDRESS] != (address_ | IdType::SMD) or read_buf[Header::SEMI_ID] != 0u or read_buf[Header::MODE] != FinishStatus::STATUS){
			std::array<uint8_t, RX_SIZE> data_tmp;

			memcpy(&data_tmp, read_buf, RX_SIZE);

			finish_queue.push_back(data_tmp);
		}else{
			smd_status_.firmware = float(read_buf[3]) / 10.0f;
			smd_status_.angle0 = read_buf[4];
			smd_status_.angle1 = read_buf[5];

			return return_status;
		}
	}
}

int MotorLib::Smd::sendStatus(uint8_t address_, uint8_t semi_id_, SmdStatus& smd_status_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};
	uint8_t read_buf[RX_SIZE] = {0u};
	int return_status;

	send_buf[0] = address_ | IdType::SMD;
	send_buf[1] = semi_id_ | IdType::SM;
	send_buf[2] = (uint8_t)this->Mode::STATUS;

	return_status = usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);

	if(return_status != TX_SIZE){
		return return_status;
	}

	while(true){
		return_status = usb->readUsb(read_buf, RX_SIZE, EndPoint::EP1, usb_timeout_);

		if(return_status != RX_SIZE){
			for(auto itr=finish_queue.begin(); itr!=finish_queue.end(); itr++){
				if((*itr)[Header::MODE] == FinishStatus::STATUS and (*itr)[Header::ADDRESS] == (address_ | IdType::SMD) and (*itr)[Header::SEMI_ID] == (semi_id_ | IdType::SM)){
					smd_status_.firmware = float((*itr)[3]) / 10.0f;
					smd_status_.angle0 = (*itr)[4];
					smd_status_.angle1 = (*itr)[5];

					finish_queue.erase(itr);

					return RX_SIZE;
				}
			}

			return return_status;
		}

		if(read_buf[Header::ADDRESS] != (address_ | IdType::SMD) or read_buf[Header::SEMI_ID] != (semi_id_ | IdType::SM) or read_buf[Header::MODE] != FinishStatus::STATUS){
			std::array<uint8_t, RX_SIZE> data_tmp;

			memcpy(&data_tmp, read_buf, RX_SIZE);

			finish_queue.push_back(data_tmp);
		}else{
			smd_status_.firmware = float(read_buf[3]) / 10.0f;
			smd_status_.angle0 = read_buf[4];
			smd_status_.angle1 = read_buf[5];

			return return_status;
		}
	}
}

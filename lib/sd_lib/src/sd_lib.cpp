#include <sd_lib.hpp>

#include <usb_connect/usb_connect.hpp>
#include <common_lib.hpp>

#include <vector>
#include <array>
#include <cstring>

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

int MotorLib::Sd::sendLimSw(uint8_t address_, Port out_port_, Port lim_port_, uint16_t first_power_, uint16_t next_power_, uint16_t timeout_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_ | IdType::SD;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)Mode::LIM_SW;
	send_buf[3] = (uint8_t)(out_port_ << 1 | lim_port_);
	send_buf[4] = (uint8_t)((first_power_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(first_power_ & 0xff);
	send_buf[6] = (uint8_t)((next_power_ >> 8) & 0xff);
	send_buf[7] = (uint8_t)(next_power_ & 0xff);
	send_buf[8] = (uint8_t)((timeout_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(timeout_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Sd::sendLimSw(uint8_t address_, uint8_t semi_id_, Port out_port_, Port lim_port_, uint16_t first_power_, uint16_t next_power_, uint16_t timeout_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = address_ | IdType::SD;
	send_buf[1] = semi_id_ | IdType::SM;
	send_buf[2] = (uint8_t)Mode::LIM_SW;
	send_buf[3] = (uint8_t)(out_port_ << 1 | lim_port_);
	send_buf[4] = (uint8_t)((first_power_ >> 8) & 0xff);
	send_buf[5] = (uint8_t)(first_power_ & 0xff);
	send_buf[6] = (uint8_t)((next_power_ >> 8) & 0xff);
	send_buf[7] = (uint8_t)(next_power_ & 0xff);
	send_buf[8] = (uint8_t)((timeout_ >> 8) & 0xff);
	send_buf[9] = (uint8_t)(timeout_ & 0xff);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Sd::sendStatus(uint8_t address_, SdStatus& sd_status_, uint32_t usb_timeout_){
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
			for(auto itr=finish_queue.begin(); itr!=finish_queue.end(); itr++){
				if((*itr)[2] == FinishStatus::STATUS and (*itr)[0] == (address_ | IdType::SD) and (*itr)[1] == 0){
					sd_status_.firmware = float((*itr)[3]) / 10.0f;
					sd_status_.lim_sw0 = bool(((*itr)[7] >> 1) & 0x01);
					sd_status_.lim_sw1 = bool((*itr)[7] & 0x01);

					finish_queue.erase(itr);

					return RX_SIZE;
				}
			}

			return return_status;
		}

		if(read_buf[0] != (address_ | IdType::SD) or read_buf[1] != 0u or read_buf[2] != FinishStatus::STATUS){
			std::array<uint8_t, RX_SIZE> status_tmp;

			memcpy(&status_tmp, read_buf, RX_SIZE);

			finish_queue.push_back(status_tmp);
		}else{
			sd_status_.firmware = float(read_buf[3]) / 10.0f;
			sd_status_.lim_sw0 = bool((read_buf[7] >> 1) & 0x01);
			sd_status_.lim_sw1 = bool(read_buf[7] & 0x01);

			return return_status;
		}
	}
}


int MotorLib::Sd::sendStatus(uint8_t address_, uint8_t semi_id_, SdStatus& sd_status_, uint32_t usb_timeout_){
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
			for(auto itr=finish_queue.begin(); itr!=finish_queue.end(); itr++){
				if((*itr)[2] == FinishStatus::STATUS and (*itr)[0] == (address_ | IdType::SD) and (*itr)[1] == (semi_id_ | IdType::SM)){
					sd_status_.firmware = float((*itr)[3]) / 4.0f;
					sd_status_.lim_sw0 = bool(((*itr)[7] >> 1) & 0x01);
					sd_status_.lim_sw1 = bool((*itr)[7] & 0x01);

					finish_queue.erase(itr);

					return RX_SIZE;
				}
			}

			return return_status;
		}

		if(read_buf[0] != (address_ | IdType::SD) or read_buf[1] != (semi_id_ | IdType::SM) or read_buf[2] != FinishStatus::STATUS){
			std::array<uint8_t, RX_SIZE> status_tmp;

			memcpy(&status_tmp, read_buf, RX_SIZE);

			finish_queue.push_back(status_tmp);
		}else{
			sd_status_.firmware = float(read_buf[3]) / 100.0f;
			sd_status_.lim_sw0 = bool((read_buf[7] >> 1) & 0x01);
			sd_status_.lim_sw1 = bool(read_buf[7] & 0x01);

			return return_status;
		}
	}
}

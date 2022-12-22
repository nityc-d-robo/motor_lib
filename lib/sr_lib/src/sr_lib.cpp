#include <sr_lib.hpp>

#include <usb_connect/usb_connect.hpp> 
#include <common_lib.hpp>

#include <vector>
#include <array>
#include <cstring>

MotorLib::Sr::Sr(UsbConnect& usb_){
	usb = &usb_;
}

void MotorLib::Sr::sendStop(void){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = (uint8_t)IdType::SR;
	send_buf[1] = (uint8_t)IdType::MASTER;
	send_buf[2] = (uint8_t)Mode::STOP;

	while(usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, 300) != TX_SIZE);
}

int MotorLib::Sr::sendStart(uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = (uint8_t)IdType::SR;
	send_buf[1] = (uint8_t)IdType::MASTER;
	send_buf[2] = (uint8_t)Mode::START;

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Sr::sendColors(uint8_t red_, uint8_t green_, uint8_t blue_, float freq_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = (uint8_t)IdType::SR;
	send_buf[1] = (uint8_t)IdType::MASTER;
	send_buf[2] = (uint8_t)Mode::COLOR;
	send_buf[4] = green_;
	send_buf[5] = red_;
	send_buf[6] = blue_;
	send_buf[7] = (uint8_t)(freq_ * 4);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Sr::sendColors(Color color_, float alpha_, float freq_, uint32_t usb_timeout_){
	return sendColors((uint8_t)(color_.red * alpha_), (uint8_t)(color_.green * alpha_), (uint8_t)(color_.blue * alpha_), freq_, usb_timeout_);
}

int MotorLib::Sr::sendColors(std::string color_code_, float freq_, uint32_t usb_timeout_){
	Color color;

	if(color_code_[0] == '#'){
		color_code_.erase(color_code_.begin());
	}

	if(color_code_.size() != 6){
		errorMotor("in sendColor, invalid string was inputed.");

		return UsbStatus::USB_OTHER_ERROR;
	}

	color.red = (uint8_t)((strtol(color_code_.c_str(), NULL, 16) >> 16) & 0xff);
	color.green = (uint8_t)((strtol(color_code_.c_str(), NULL, 16) >> 8) & 0xff);
	color.blue = (uint8_t)(strtol(color_code_.c_str(), NULL, 16) & 0xff);

	return sendColors(color, 1.0f, freq_, usb_timeout_);
}

int MotorLib::Sr::sendStatus(SrStatus& sr_status_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};
	uint8_t read_buf[RX_SIZE] = {0u};
	int return_status = UsbStatus::USB_OTHER_ERROR;

	send_buf[0] = (uint8_t)IdType::SR;
	send_buf[1] = (uint8_t)IdType::MASTER;
	send_buf[2] = (uint8_t)Mode::STATUS;

	return_status = usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);

	if(return_status != TX_SIZE){
		return return_status;
	}

	while(true){
		return_status = usb->readUsb(read_buf, RX_SIZE, EndPoint::EP1, usb_timeout_);

		if(return_status != RX_SIZE){
			for(auto itr=finish_queue.begin(); itr!=finish_queue.end(); itr++){
				if((*itr)[Header::ADDRESS] == IdType::SR and (*itr)[Header::MODE] == FinishStatus::STATUS){
					sr_status_.firmware = float((*itr)[3]) / 10.0f;
					sr_status_.voltage = bool((*itr)[4] & 0x01);
					sr_status_.freq = float(((*itr)[4] >> 1) & 0x7f);
					sr_status_.color.red = (*itr)[5];
					sr_status_.color.green = (*itr)[6];
					sr_status_.color.blue = (*itr)[7];

					finish_queue.erase(itr);

					return RX_SIZE;
				}
			}

			return return_status;
		}

		if(read_buf[Header::ADDRESS] != IdType::SR or read_buf[Header::MODE] != FinishStatus::STATUS){
			std::array<uint8_t, RX_SIZE> status_tmp;

			memcpy(&status_tmp, read_buf, RX_SIZE);

			finish_queue.push_back(status_tmp);
		}else{
			sr_status_.firmware = float(read_buf[3]) / 10.0f;
			sr_status_.voltage = bool(read_buf[4] & 0x01);
			sr_status_.freq = float((read_buf[4] >> 1) & 0x7f) / 4.0f;
			sr_status_.color.red = read_buf[5];
			sr_status_.color.green = read_buf[6];
			sr_status_.color.blue = read_buf[7];

			return return_status;
		}
	}
}

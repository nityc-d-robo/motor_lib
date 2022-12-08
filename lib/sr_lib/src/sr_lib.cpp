#include <sr_lib.hpp>

#include <usb_connect/usb_connect.hpp>
#include <common_lib.hpp>

MotorLib::Sr::Sr(SerialConnect& serial_){
	serial = &serial_;
}

void MotorLib::Sr::sendStop(void){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = (uint8_t)IdType::SR;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)Mode::STOP;

	while(usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, 300) != TX_SIZE);
}

int MotorLib::Sr::sendStart(uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = (uint8_t)IdType::SR;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)Mode::START;

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Sr::sendColors(uint8_t red_, uint8_t green_, uint8_t blue_, float freq_, uint32_t usb_timeout_){
	uint8_t send_buf[TX_SIZE] = {0u};

	send_buf[0] = (uint8_t)IdType::SR;
	send_buf[1] = 0u;
	send_buf[2] = (uint8_t)Mode::Color;
	send_buf[4] = green_;
	send_buf[5] = red_;
	send_buf[6] = blue_;
	send_buf[7] = (uint8_t)(freq_ * 4);

	return usb->writeUsb(send_buf, TX_SIZE, EndPoint::EP1, usb_timeout_);
}

int MotorLib::Sr::sendColors(Color color_, float alpha_, float freq_, uint32_t usb_timeout_){
	return sendColors((uint8_t)(color_.red * alpha_), (uint8_t)(color_.green * alpha_), (uint8_t)(color_.blue * aplha_), freq_, usb_timeout);
}

int MotorLib::Sr::sendColor(std::string color_code_, float freq_, uint32_t usb_timeout_){
	Color color;

	if(color_code_[0] == '#'){
		color_code_.erase(color_code_.begin());
	}

	if(color_code_.size() != 6){
		errorMotor("in sendColor, invalid string was inputed.");

		return UsbStatus::USB_OTHER_ERROR;
	}


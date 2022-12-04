#pragma once

#include <serial_connect/serial_connect.hpp>

typedef struct MdStatus{
	float firmware;
	int32_t encoder_cnt;
	bool lim_sw1, lim_sw2;
}MdStatus;

class Md{
	public:
		Md(SerialConnect& serial_);
		void sendPwm(uint16_t address_, bool phase_, uint16_t power_, uint32_t usb_timeout_);
		void sendPwm(uint16_t address_, uint16_t semi_id_, bool phase_, uint16_t power_, uint32_t usb_timeout_);
		void sendSpeed(uint16_t address_, bool phase_, uint16_t speed_, uint16_t end_, uint16_t timeout_, uint32_t usb_timeout_);
		void sendSpeed(uint16_t address_, uint16_t semi_id_, bool phase_, uint16_t speed_, uint16_t end_, uint16_t timeout_, uint32_t usb_timeout_);
		void sendAngle(uint16_t address_, bool phase_, uint16_t speed_, uint16_t angle_, uint16_t timeout_, uint32_t usb_timeout_);
		void sendAngle(uint16_t address_, uint16_t semi_id_, bool phase_, uint16_t speed_, uint16_t angle_, uint16_t timeout_, uint32_t usb_timeout_);
		void sendLimSw(uint16_t address_, bool phase_, uint16_t speed_, bool port_, uint32_t usb_timeout_);
		void sendLimSw(uint16_t address_, uint16_t semi_id_, bool phase_, uint16_t speed_, bool port_, uint32_t usb_timeout_);
		void sendInit(uint16_t address_, bool angle_reset_, uint16_t max_rpm_, uint16_t max_torque_, uint16_t gear_ratio_, uint32_t usb_timeout_);
		void sendInit(uint16_t address_, uint16_t semi_id_, bool angle_reset_, uint16_t max_rpm_, uint16_t max_torque_, uint16_t gear_ratio_, uint32_t usb_timeout_);
		void sendStatus(uint16_t address_, MdStatus& md_status_, uint32_t usb_timeout_);
		void sendStatus(uint16_t address_, uint16_t semi_id_, MdStatus& md_status_, uint32_t usb_timeout_);
	private:
		SerialConnect* serial;
		typedef enum Mode{
			INIT, STATUS, PWM, SPEED, ANGLE, LIM_SW
		}Mode;
};

use std::time::Duration;
use rusb::{constants::{LIBUSB_ENDPOINT_IN, LIBUSB_ENDPOINT_OUT}, DeviceHandle, Error, GlobalContext};

use crate::{IdType, EndPont};

#[allow(non_snake_case)]
pub mod Mode {
    pub const INIT: u8 = 0;
    pub const STATUS: u8 = 1;
    pub const PWM: u8 = 2;
    pub const SPEED: u8 = 3;
    pub const ANGLE: u8 = 4;
    pub const LIM_SW: u8 = 5;
}

#[derive(Debug)]
pub struct LimSwStatus {
    pub limsw_0: bool,
    pub limsw_1: bool,
}

#[derive(Debug)]
pub struct MdStatus {
    pub address: u8,
    pub semi_id: u8,
    pub angle: i16,
    pub speed: i16,
    pub limsw: LimSwStatus,
}

fn rpm_to_count(rpm: i16) -> i16 {
    let count = 8192.0 * (rpm as f64) / (12.0 * 1000.0);
    return count.round() as i16
}
fn count_to_rpm(rpm: i16) -> i16 {
    let rpm = (12.0 * 1000.0 * rpm as f64) / 8192.0;
    return rpm.round() as i16
}

pub fn send_pwm(handle_: &DeviceHandle<GlobalContext>, address_: u8, power_: i16) -> Result<MdStatus, Error>{
    let send_buf: [u8; 8] = [
        address_,
        IdType::MASTER,
        Mode::PWM,
        0,
        ((power_ >> 8) & 0xff) as u8,
        (power_ & 0xff) as u8,
        0,
        0
    ];
    handle_.write_bulk(LIBUSB_ENDPOINT_OUT | EndPont::EP1, &send_buf, Duration::from_millis(5000)).unwrap();
    return receive_status(handle_, address_)
}

pub fn send_speed(handle_: &DeviceHandle<GlobalContext>, address_: u8, velocity_: i16) -> Result<MdStatus, Error>{
    if velocity_ == 0 {
        send_pwm(handle_, address_, 0)?;
    } else {
    let count = rpm_to_count(velocity_);
    let send_buf: [u8; 8] = [
        address_,
        IdType::MASTER,
        Mode::SPEED,
        0,
        ((count >> 8) & 0xff) as u8,
        (count & 0xff) as u8,
        0,
        0
    ];
        handle_.write_bulk(LIBUSB_ENDPOINT_OUT | EndPont::EP1, &send_buf, Duration::from_millis(5000))?;
    }
    return receive_status(handle_, address_)
}

pub fn send_angle(handle_: &DeviceHandle<GlobalContext>, address_: u8, angle_: i16) -> Result<MdStatus, Error>{
    let angle_abs = angle_.abs();
    let send_buf: [u8; 8] = [
        address_,
        IdType::MASTER,
        Mode::SPEED,
        if angle_ >= 0 {0} else {1},
        ((angle_abs >> 8) & 0xff) as u8,
        (angle_abs & 0xff) as u8,
        0,
        0
    ];
    handle_.write_bulk(LIBUSB_ENDPOINT_OUT | EndPont::EP1, &send_buf, Duration::from_millis(5000)).unwrap();
    return receive_status(handle_, address_)
}

pub fn send_limsw(handle_: &DeviceHandle<GlobalContext>, address_: u8, port_: u8, power_: i16, after_power_: i16) -> Result<MdStatus, Error>{
    let send_buf: [u8; 8] = [
        address_,
        IdType::MASTER,
        Mode::LIM_SW,
        port_,
        ((power_ >> 8) & 0xff) as u8,
        (power_ & 0xff) as u8,
        ((after_power_ >> 8) & 0xff) as u8,
        (after_power_ & 0xff) as u8,
    ];
    handle_.write_bulk(LIBUSB_ENDPOINT_OUT | EndPont::EP1, &send_buf, Duration::from_millis(5000)).unwrap();
    return receive_status(handle_, address_)
}

#[allow(unused_variables)]
pub fn receive_status(handle_: &DeviceHandle<GlobalContext>, address_: u8) -> Result<MdStatus, Error>{
    let mut receive_buf = [0;8]; // MD側がデータ返送に対応するまでの仮実装
    loop {
        handle_.read_bulk(LIBUSB_ENDPOINT_IN | EndPont::EP1, &mut receive_buf, Duration::from_millis(5000)).unwrap();
        if address_ == receive_buf[0] {
            let rpm = count_to_rpm((receive_buf[4] as i16) << 8 | (receive_buf[5] as i16));
            return Ok(MdStatus{
                address: receive_buf[0],
                semi_id: receive_buf[1],
                angle: ((receive_buf[2] as i16) << 8 | (receive_buf[3] as i16)),
                speed: rpm,
                limsw: LimSwStatus{
                    limsw_0: receive_buf[6] == 1,
                    limsw_1: receive_buf[7] == 1,
                }
            });
        }
    }
}

#[cfg(test)]
mod tests {
    use std::{thread, time::Duration};

    use crate::{init_usb_handle, md, send_emergency};

    #[test]
    fn motor_rotation() {
        let handle = init_usb_handle(0x483, 0x5740, 1).unwrap();
        // エラーハンドリング
        let return_status = 
            loop {
                match md::send_speed(&handle, 0x00, 400) {
                    Ok(t) => {
                        break t
                    },
                    Err(_) => {
                        // 何らかのエラー処理
                    }
                };
            };
        println!("{:?}", return_status);
        loop {
            let return_status = md::receive_status(&handle, 0x00).unwrap();
            println!("{:?}", return_status);
        }
    }
    #[test]
    fn omni_control_test() {
        let handle = init_usb_handle(0x483, 0x5740, 1).unwrap();
        // エラーハンドリング
        for i in 0..500 {
            println!("{i}");
            let return_status = md::send_speed(&handle, 0x00, i).unwrap();
            let return_status = md::send_speed(&handle, 0x01, i).unwrap();
            let return_status = md::send_speed(&handle, 0x02, i).unwrap();
            println!("{:?}", return_status);
            thread::sleep(Duration::from_millis(100));
        }
        send_emergency(&handle);
    }
    #[test]
    fn stop_all() {
        let handle = init_usb_handle(0x483, 0x5740, 1).unwrap();
        send_emergency(&handle);
    }
}

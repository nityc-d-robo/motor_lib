use std::time::Duration;
use rusb::{DeviceHandle, GlobalContext, constants::{LIBUSB_ENDPOINT_IN, LIBUSB_ENDPOINT_OUT}};

use crate::{IdType, EndPont};

#[allow(non_snake_case)]
pub mod Mode {
    pub static STATUS: u8 = 0;
    pub static POWER: u8 = 1;
    pub static LIM_SW: u8 = 2;
    pub static SINGLE_POWER: u8 = 3;
}

#[derive(Debug)]
pub struct LimSwStatus {
    pub limsw_0: bool,
    pub limsw_1: bool,
}

#[derive(Debug)]
pub struct SdStatus {
    pub address: u8,
    pub semi_id: u8,
    pub angle: i16,
    pub speed: i16,
    pub limsw: LimSwStatus,
}

pub fn send_power(handle_: &DeviceHandle<GlobalContext>, address_: u8, port_: u8, power_: i16) -> SdStatus{
    let power_abs = power_.abs();
    let send_buf: [u8; 8] = [
        address_ | IdType::SD,
        IdType::MASTER,
        Mode::SINGLE_POWER,
        port_,
        ((power_abs >> 8) & 0xff) as u8,
        (power_abs & 0xff) as u8,
        0,
        0
    ];
    handle_.write_bulk(LIBUSB_ENDPOINT_OUT | EndPont::EP1, &send_buf, Duration::from_millis(5000)).unwrap();
    return receive_status(handle_, address_)
}

pub fn send_powers(handle_: &DeviceHandle<GlobalContext>, address_: u8, power0_: i16, power1_ :i16) -> SdStatus{
    let power0_abs = power0_.abs();
    let power1_abs = power1_.abs();

    let send_buf: [u8; 8] = [
        address_ | IdType::SD,
        IdType::MASTER,
        Mode::POWER,
        0,
        ((power0_abs >> 8) & 0xff) as u8,
        (power0_abs & 0xff) as u8,
        ((power1_abs >> 8) & 0xff) as u8,
        (power1_abs & 0xff) as u8,
    ];
    handle_.write_bulk(LIBUSB_ENDPOINT_OUT | EndPont::EP1, &send_buf, Duration::from_millis(5000)).unwrap();
    return receive_status(handle_, address_)
}

#[allow(unused_variables)]
pub fn receive_status(handle_: &DeviceHandle<GlobalContext>, address_: u8) -> SdStatus{
    let receive_buf = [0;8]; // SD側がデータ返送に対応するまでの仮実装
    loop {
        // handle_.read_bulk(LIBUSB_ENDPOINT_IN | EndPont::EP1, &mut receive_buf, Duration::from_millis(5000)).unwrap();
        // if (address_ | IdType::SD) == receive_buf[0] {
            return SdStatus{
                address: receive_buf[0],
                semi_id: receive_buf[1],
                angle: ((receive_buf[2] as i16) << 8 | (receive_buf[3] as i16)),
                speed: ((receive_buf[4] as i16) << 8 | (receive_buf[5] as i16)),
                limsw: LimSwStatus{
                    limsw_0: receive_buf[6] == 1,
                    limsw_1: receive_buf[7] == 1,
                }
            }
        // }
    }
}
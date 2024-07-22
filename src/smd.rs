use std::time::Duration;
use rusb::{DeviceHandle, GlobalContext, constants::{LIBUSB_ENDPOINT_IN, LIBUSB_ENDPOINT_OUT}};

use crate::{IdType, EndPont};

#[allow(non_snake_case)]
pub mod Mode {
    pub static STATUS: u8 = 0;
    pub static ANGLE: u8 = 1;
    pub static ANGLES: u8 = 2;
}

#[derive(Debug)]
pub struct SmdStatus {
    pub address: u8,
    pub semi_id: u8,
    pub angle: i16,
    pub speed: i16,
}

pub fn send_angle(handle_: &DeviceHandle<GlobalContext>, address_: u8, port_: u8, angle_: u8) -> SmdStatus{
    let send_buf: [u8; 8] = [
        address_ | IdType::SMD,
        IdType::MASTER,
        Mode::ANGLE,
        port_,
        angle_,
        0,
        0,
        0
    ];
    handle_.write_bulk(LIBUSB_ENDPOINT_OUT | EndPont::EP1, &send_buf, Duration::from_millis(5000)).unwrap();
    return receive_status(handle_, address_)
}

pub fn send_angles(handle_: &DeviceHandle<GlobalContext>, address_: u8, angle0_: u8, angle1_ :u8) -> SmdStatus{
    let send_buf: [u8; 8] = [
        address_ | IdType::SD,
        IdType::MASTER,
        Mode::ANGLES,
        0,
        angle0_,
        angle1_,
        0,
        0
    ];
    handle_.write_bulk(LIBUSB_ENDPOINT_OUT | EndPont::EP1, &send_buf, Duration::from_millis(5000)).unwrap();
    return receive_status(handle_, address_)
}

pub fn receive_status(handle_: &DeviceHandle<GlobalContext>, address_: u8) -> SmdStatus{
    let mut receive_buf = [0;8];
    loop {
        // handle_.read_bulk(LIBUSB_ENDPOINT_IN | EndPont::EP1, &mut receive_buf, Duration::from_millis(5000)).unwrap();
        // if (address_ | IdType::SD) == receive_buf[0] {
            return SmdStatus{
                address: receive_buf[0],
                semi_id: receive_buf[1],
                angle: ((receive_buf[2] as i16) << 8 | (receive_buf[3] as i16)),
                speed: ((receive_buf[4] as i16) << 8 | (receive_buf[5] as i16)),
            }
        // }
    }
}
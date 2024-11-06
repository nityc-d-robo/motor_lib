use std::time::Duration;
use rusb::{constants::{LIBUSB_ENDPOINT_IN, LIBUSB_ENDPOINT_OUT}, DeviceHandle, Error, GlobalContext};

use crate::{IdType, EndPont};

#[allow(non_snake_case)]
pub mod Mode {
    pub const STATUS: u8 = 0;
    pub const STOP: u8 = 1;
    pub const START: u8 = 2;
    pub const COLOR: u8 = 3;
}

#[derive(Debug)]

pub struct Color {
    pub red: u8,
    pub green: u8,
    pub blue: u8,
}

#[derive(Debug)]
pub struct SrStatus {
    pub address: f32,
    pub voltage: bool,
    pub color: Color,
    pub freq: f32,
}

pub fn send_stop(handle_: &DeviceHandle<GlobalContext>) {
    let send_buf: [u8; 8] = [
        IdType::SR,
        IdType::MASTER,
        Mode::STOP,
        0,
        0,
        0,
        0,
        0
    ];
    handle_.write_bulk(LIBUSB_ENDPOINT_OUT | EndPont::EP1, &send_buf, Duration::from_millis(5000)).unwrap();
}
pub fn send_start(handle_: &DeviceHandle<GlobalContext>, timeout: u16) {
    let send_buf: [u8; 8] = [
        IdType::SR,
        IdType::MASTER,
        Mode::START,
        0,
        0,
        0,
        0,
        0
    ];
    handle_.write_bulk(LIBUSB_ENDPOINT_OUT | EndPont::EP1, &send_buf, Duration::from_millis(timeout.into())).unwrap();
}
pub fn send_colors(handle_: &DeviceHandle<GlobalContext>, red: u8, green: u8, blue: u8, freq: f32, timeout: u16) {
    let send_buf: [u8; 8] = [
        IdType::SR,
        IdType::MASTER,
        Mode::COLOR,
        0,
        green,
        red,
        blue,
        (freq * 4.0) as u8
    ];
    handle_.write_bulk(LIBUSB_ENDPOINT_OUT | EndPont::EP1, &send_buf, Duration::from_millis(timeout.into())).unwrap();
}
use std::time::Duration;

use crate::{device_type, usb};

pub mod mode {
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

pub fn send_stop(handle: &impl usb::USBHandleTrait) {
    let send_buf: [u8; 8] = [
        device_type::SR,
        device_type::MASTER,
        mode::STOP,
        0,
        0,
        0,
        0,
        0,
    ];
    handle
        .write_bulk(&send_buf, Duration::from_millis(5000))
        .unwrap();
}
pub fn send_start(handle: &impl usb::USBHandleTrait, timeout: u16) {
    let send_buf: [u8; 8] = [
        device_type::SR,
        device_type::MASTER,
        mode::START,
        0,
        0,
        0,
        0,
        0,
    ];
    handle
        .write_bulk(&send_buf, Duration::from_millis(timeout.into()))
        .unwrap();
}
pub fn send_colors(
    handle: &impl usb::USBHandleTrait,
    red: u8,
    green: u8,
    blue: u8,
    freq: f32,
    timeout: u16,
) {
    let send_buf: [u8; 8] = [
        device_type::SR,
        device_type::MASTER,
        mode::COLOR,
        0,
        green,
        red,
        blue,
        (freq * 4.0) as u8,
    ];
    handle
        .write_bulk(&send_buf, Duration::from_millis(timeout.into()))
        .unwrap();
}

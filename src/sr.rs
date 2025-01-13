//! This module provides functions to control SR devices using USB communication.

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

/// Sends a stop command to the SR device.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the USBHandleTrait.
///
/// # Example
///
/// Sample code to send a stop command to the SR device.
/// ```rust
/// use motor_lib::{USBHandle, sr};
/// fn main() {
///     let handle = USBHandle::new(0x483, 0x5740, 1);
///     sr::send_stop(&handle);
/// }
/// ```
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

/// Sends a start command to the SR device with a specified timeout.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the USBHandleTrait.
/// * `timeout` - The duration to wait for the operation to complete.
///
/// # Example
///
/// Sample code to send a start command to the SR device with a timeout of 1000 milliseconds.
/// ```rust
/// use motor_lib::{USBHandle, sr};
/// fn main() {
///     let handle = USBHandle::new(0x483, 0x5740, 1);
///     sr::send_start(&handle, 1000);
/// }
/// ```
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

/// Sends color settings to the LED strip connected to the SR device.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the USBHandleTrait.
/// * `red` - The red color intensity.
/// * `green` - The green color intensity.
/// * `blue` - The blue color intensity.
/// * `freq` - The frequency of the color change.
/// * `timeout` - The duration to wait for the operation to complete.
///
/// # Example
///
/// Sample code to set the color of the LED strip connected to the SR device to red, green, and blue with a timeout of 1000 milliseconds.
/// ```rust
/// use motor_lib::{USBHandle, sr};
/// fn main() {
///     let handle = USBHandle::new(0x483, 0x5740, 1);
///     sr::send_colors(&handle, 255, 0, 0, 0.0, 1000);
/// }
/// ```
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

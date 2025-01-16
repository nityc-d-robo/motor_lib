//! This module provides functions to control SD devices using USB communication.

use std::time::Duration;

use crate::{device_type, HandleTrait};

pub mod mode {
    pub const STATUS: u8 = 0;
    pub const POWER: u8 = 1;
    pub const LIM_SW: u8 = 2;
    pub const SINGLE_POWER: u8 = 3;
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
    pub port_0: i16,
    pub port_1: i16,
    pub limsw: LimSwStatus,
}

/// Sends a command to set the solenoid state on the specified SD port.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the USBHandleTrait.
/// * `address` - The address of the SD device.
/// * `port` - The port number.
/// * `power` - The power to set.
///
/// # Returns
///
/// A result containing the status of the SD device or a Error.
///
/// # Example
///
/// Sets the state of the solenoid connected to port 0 of SD at address 0x10 to HIGH.
/// It is recommended that the number set to the `power` argument be 0 or 1000.
/// ```rust
/// use motor_lib::{USBHandle, Error, sd};
/// fn main() -> Result<(), Error> {
///     let handle = USBHandle::new(0x483, 0x5740, 1);
///     sd::send_power(&handle, 0x10, 0, 1000)?;
///     Ok(())
/// }
/// ```
pub fn send_power(
    handle: &impl HandleTrait,
    address: u8,
    port: u8,
    power: i16,
) -> Result<SdStatus, crate::Error> {
    let power_abs = power.abs();
    let send_buf: [u8; 8] = [
        address | device_type::SD,
        device_type::MASTER,
        mode::SINGLE_POWER,
        port,
        ((power_abs >> 8) & 0xff) as u8,
        (power_abs & 0xff) as u8,
        0,
        0,
    ];
    handle.write_bulk(&send_buf, Duration::from_millis(5000))?;
    return receive_status(handle, address);
}

/// Sends a command to set the solenoid state on the specified SD.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the USBHandleTrait.
/// * `address` - The address of the SD device.
/// * `power_0` - The power to set for port 0.
/// * `power_1` - The power to set for port 1.
///
/// # Returns
///
/// A result containing the status of the SD device or a Error.
///
/// # Example
///
/// Sets the state of solenoids connected to ports 0 and 1 of SD to LOW and HIGH.
/// It is recommended that the number set to the `power_0` and `power_1` arguments be 0 or 1000.
/// ```rust
/// use motor_lib::{USBHandle, Error, sd};
/// fn main() -> Result<(), Error> {
///     let handle = USBHandle::new(0x483, 0x5740, 1);
///     sd::send_powers(&handle, 0x10, 0, 1000)?;
///     Ok(())
/// }
/// ```
pub fn send_powers(
    handle: &impl HandleTrait,
    address: u8,
    power_0: i16,
    power_1: i16,
) -> Result<SdStatus, crate::Error> {
    let power0_abs = power_0.abs();
    let power1_abs = power_1.abs();

    let send_buf: [u8; 8] = [
        address | device_type::SD,
        device_type::MASTER,
        mode::POWER,
        0,
        ((power0_abs >> 8) & 0xff) as u8,
        (power0_abs & 0xff) as u8,
        ((power1_abs >> 8) & 0xff) as u8,
        (power1_abs & 0xff) as u8,
    ];
    handle.write_bulk(&send_buf, Duration::from_millis(5000))?;
    return receive_status(handle, address);
}

/// Receive a data from the specified SD device.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the USBHandleTrait.
/// * `address` - The address of the SD device.
///
/// # Returns
///
/// A result containing the status of the SD device or a Error.
///
/// # Example
///
/// Sample code to retrieve status data from the SD at address 0x10.
/// ```rust
/// use motor_lib::{USBHandle, Error, sd};
/// fn main() -> Result<(), Error> {
///     let handle = USBHandle::new(0x483, 0x5740, 1);
///     let status = sd::receive_status(&handle, 0x10)?;
///     println!("{:?}", status);
///     Ok(())
/// }
/// ```
pub fn receive_status(handle: &impl HandleTrait, address: u8) -> Result<SdStatus, crate::Error> {
    let mut receive_buf = [0; 8];
    loop {
        handle.read_bulk(&mut receive_buf, Duration::from_millis(5000))?;
        if (address | device_type::SD) == receive_buf[0] {
            return Ok(SdStatus {
                address: receive_buf[0],
                semi_id: receive_buf[1],
                port_0: ((receive_buf[2] as i16) << 8 | (receive_buf[3] as i16)),
                port_1: ((receive_buf[4] as i16) << 8 | (receive_buf[5] as i16)),
                limsw: LimSwStatus {
                    limsw_0: receive_buf[6] == 1,
                    limsw_1: receive_buf[7] == 1,
                },
            });
        }
    }
}

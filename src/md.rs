//! This module provides functions to control MD devices using USB communication.

use std::time::Duration;

use crate::{device_type, HandleTrait};

pub mod mode {
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

/// Sends a command to set the PWM duty cycle on the specified MD device.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the USBHandleTrait.
/// * `address` - The address of the MD device.
/// * `power` - The PWM duty cycle to set.
///
/// # Returns
///
/// A result containing the status of the MD device or a Error.
///
/// # Example
///
/// Sample code to rotate a motor connected to the MD at address 0x00 at a PWM duty cycle of 1000.  
/// This sample code retrieves information such as the rotation speed after running the motor.
/// ```rust
/// use motor_lib::{USBHandle, Error, md};
/// fn main() {
///    let handle = USBHandle::new(0x483, 0x5740, 1);
///    let return_status =
///    loop {
///         match md::send_pwm(&handle, 0x00, 1000) {
///             Ok(t) => {
///                 break t;
///             },
///             Err(e) => {}
///         }
///    };
///    println!("{:?}", return_status);
/// }
/// ```
pub fn send_pwm(
    handle: &impl HandleTrait,
    address: u8,
    power: i16,
) -> Result<MdStatus, crate::Error> {
    let send_buf: [u8; 8] = [
        address,
        device_type::MASTER,
        mode::PWM,
        0,
        ((power >> 8) & 0xff) as u8,
        (power & 0xff) as u8,
        0,
        0,
    ];
    handle.write_bulk(&send_buf, Duration::from_millis(5000))?;
    return receive_status(handle, address);
}

/// Sends a command to set the rotation speed on the specified MD device.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the USBHandleTrait.
/// * `address` - The address of the MD device.
/// * `velocity` - The rotation speed to set.
///
/// # Returns
///
/// A result containing the status of the MD device or a Error.
///
/// # Example
///
/// Sample code to rotate a motor connected to the MD at address 0x00 at 100 rpm.
/// ```rust
/// use motor_lib::{USBHandle, Error, md};
/// use std::time::Duration;
/// fn main() -> Result<(), Error> {
///     let handle = USBHandle::new(0x483, 0x5740, 1);
///     md::send_speed(&handle, 0x00, 100)?;
///     Ok(())
/// }
/// ```
pub fn send_speed(
    handle: &impl HandleTrait,
    address: u8,
    velocity: i16,
) -> Result<MdStatus, crate::Error> {
    if velocity == 0 {
        send_pwm(handle, address, 0)?;
    } else {
        let send_buf: [u8; 8] = [
            address,
            device_type::MASTER,
            mode::SPEED,
            0,
            ((velocity >> 8) & 0xff) as u8,
            (velocity & 0xff) as u8,
            0,
            0,
        ];
        handle.write_bulk(&send_buf, Duration::from_millis(5000))?;
    }
    return receive_status(handle, address);
}

/// Sends a command to set the angle on the specified MD device.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the USBHandleTrait.
/// * `address` - The address of the MD device.
/// * `angle` - The angle to set.
///
/// # Returns
///
/// A result containing the status of the MD device or a Error.
///
/// # Example
///
/// Sample code to set the angle of a motor connected to the MD at address 0x00 to 90 degrees.
/// ```rust
/// use motor_lib::{USBHandle, Error, md};
/// fn main() -> Result<(), Error> {
///     let handle = USBHandle::new(0x483, 0x5740, 1);
///     md::send_angle(&handle, 0x00, 90)?;
///     Ok(())
/// }
/// ```
pub fn send_angle(
    handle: &impl HandleTrait,
    address: u8,
    angle: i16,
) -> Result<MdStatus, crate::Error> {
    let send_buf: [u8; 8] = [
        address,
        device_type::MASTER,
        mode::ANGLE,
        0,
        ((angle >> 8) & 0xff) as u8,
        (angle & 0xff) as u8,
        0,
        0,
    ];
    handle.write_bulk(&send_buf, Duration::from_millis(5000))?;
    return receive_status(handle, address);
}

/// Sends a command to set the duty cycle on before and after pressing the limit switch.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the USBHandleTrait.
/// * `address` - The address of the MD device.
/// * `port` - The port number.
/// * `power` - The power before pressing the limit switch.
/// * `after_power` - The power after pressing the limit switch.
///
/// # Returns
///
/// A result containing the status of the MD device or a Error.
///
/// # Example
///
/// Sample code to set the duty cycle before and after pressing the limit switch on the MD at address 0x00.
/// ```rust
/// use motor_lib::{USBHandle, Error, md};
/// fn main() -> Result<(), Error> {
///     let handle = USBHandle::new(0x483, 0x5740, 1);
///     md::send_limsw(&handle, 0x00, 1, 1000, 500)?;
///     Ok(())
/// }
/// ```
pub fn send_limsw(
    handle: &impl HandleTrait,
    address: u8,
    port: u8,
    power: i16,
    after_power: i16,
) -> Result<MdStatus, crate::Error> {
    let send_buf: [u8; 8] = [
        address,
        device_type::MASTER,
        mode::LIM_SW,
        port,
        ((power >> 8) & 0xff) as u8,
        (power & 0xff) as u8,
        ((after_power >> 8) & 0xff) as u8,
        (after_power & 0xff) as u8,
    ];
    handle.write_bulk(&send_buf, Duration::from_millis(5000))?;
    return receive_status(handle, address);
}

/// Receive a data from the specified MD device.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the USBHandleTrait.
/// * `address` - The address of the MD device.
///
/// # Returns
///
/// A result containing the status of the MD device or a Error.
///
/// # Example
///
/// Sample code to rotate a motor at 100 rpm and continuously retrieve rotation speed data.
/// ```rust
/// use motor_lib::{USBHandle, Error, md};
/// use std::thread::sleep;
/// use std::time::Duration;
/// fn main() -> Result<(), Error> {
///     let handle = USBHandle::new(0x483, 0x5740, 1);
///     md::send_speed(&handle, 0x00, 100)?;
///     std::thread::sleep(Duration::from_secs(1));
///     let status = md::receive_status(&handle, 0x00)?;
///     println!("{:?}", status);
///     Ok(())
/// }
/// ```
pub fn receive_status(handle: &impl HandleTrait, address: u8) -> Result<MdStatus, crate::Error> {
    let mut receive_buf = [0; 8];
    loop {
        handle.read_bulk(&mut receive_buf, Duration::from_millis(5000))?;
        if address == receive_buf[0] {
            let rpm = (receive_buf[4] as i16) << 8 | (receive_buf[5] as i16);
            return Ok(MdStatus {
                address: receive_buf[0],
                semi_id: receive_buf[1],
                angle: ((receive_buf[2] as i16) << 8 | (receive_buf[3] as i16)),
                speed: rpm,
                limsw: LimSwStatus {
                    limsw_0: receive_buf[6] == 1,
                    limsw_1: receive_buf[7] == 1,
                },
            });
        }
    }
}

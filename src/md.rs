use std::time::Duration;

use crate::{device_type, usb};

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
/// ## Example
/// Sample code to rotate a motor connected to the MD at address 0x00 at a PWM duty cycle of 1000.  
/// This sample code retrieves information such as the rotation speed after running the motor.
/// ```rust
/// use motor_lib::{USBHandle, USBError, md};
/// fn main() {
///    let handle = USBHandle;
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
    handle: &impl usb::USBHandleTrait,
    address: u8,
    power: i16,
) -> Result<MdStatus, crate::USBError> {
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
    handle
        .write_bulk(&send_buf, Duration::from_millis(5000))
        .unwrap();
    return receive_status(handle, address);
}

/// Sends a command to set the rotation speed on the specified MD device.
/// ## Example
/// Sample code to rotate a motor connected to the MD at address 0x00 at 100 rpm.
/// ```rust
/// use motor_lib::{USBHandle, USBError, md};
/// use std::time::Duration;
/// fn main() -> Result<(), USBError> {
///     let handle = USBHandle;
///     md::send_speed(&handle, 0x00, 100)?;
///     Ok(())
/// }
/// ```
pub fn send_speed(
    handle: &impl usb::USBHandleTrait,
    address: u8,
    velocity: i16,
) -> Result<MdStatus, crate::USBError> {
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

pub fn send_angle(
    handle: &impl usb::USBHandleTrait,
    address: u8,
    angle: i16,
) -> Result<MdStatus, crate::USBError> {
    let angle_abs = angle.abs();
    let send_buf: [u8; 8] = [
        address,
        device_type::MASTER,
        mode::SPEED,
        if angle >= 0 { 0 } else { 1 },
        ((angle_abs >> 8) & 0xff) as u8,
        (angle_abs & 0xff) as u8,
        0,
        0,
    ];
    handle
        .write_bulk(&send_buf, Duration::from_millis(5000))
        .unwrap();
    return receive_status(handle, address);
}

/// Sends a command to set the duty cycle on before and after pressing the limit switch.
pub fn send_limsw(
    handle: &impl usb::USBHandleTrait,
    address: u8,
    port: u8,
    power: i16,
    after_power: i16,
) -> Result<MdStatus, crate::USBError> {
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
    handle
        .write_bulk(&send_buf, Duration::from_millis(5000))
        .unwrap();
    return receive_status(handle, address);
}

/// Receive a data from the specified MD device.
/// ## Example
/// Sample code to rotate a motor at 100 rpm and continuously retrieve rotation speed data.
/// ```rust
/// use motor_lib::{USBHandle, USBError, md};
/// use std::thread::sleep;
/// use std::time::Duration;
/// fn main() -> Result<(), USBError> {
///     let handle = USBHandle;
///     md::send_speed(&handle, 0x00, 100)?;
///     std::thread::sleep(Duration::from_secs(1));
///     let status = md::receive_status(&handle, 0x00)?;
///     // 値が一定の範囲に収まっていれば成功というような処理
///     Ok(())
/// }
/// ```
pub fn receive_status(
    handle: &impl usb::USBHandleTrait,
    address: u8,
) -> Result<MdStatus, crate::USBError> {
    let mut receive_buf = [0; 8];
    loop {
        handle
            .read_bulk(&mut receive_buf, Duration::from_millis(5000))
            .unwrap();
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

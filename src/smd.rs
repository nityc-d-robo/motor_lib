use std::time::Duration;

use crate::{device_type, usb};

pub mod mode {
    pub const STATUS: u8 = 0;
    pub const ANGLE: u8 = 1;
    pub const ANGLES: u8 = 2;
}

#[derive(Debug)]
pub struct SmdStatus {
    pub address: u8,
    pub semi_id: u8,
    pub angle: i16,
    pub speed: i16,
}

/// Sends a command to set the angle on the specified SMD device.
/// ## Example
/// Sample code to set the angle of a motor connected to the SMD at address 0x20 and port 1 to 45 degrees.
/// ```rust
/// use motor_lib::{USBHandle, USBError, smd};
/// fn main() -> Result<(), USBError> {
///     let handle = USBHandle;
///     smd::send_angle(&handle, 0x20, 1, 45)?;
///     Ok(())
/// }
/// ```
pub fn send_angle(
    handle: &impl usb::USBHandleTrait,
    address: u8,
    port: u8,
    angle: u8,
) -> Result<SmdStatus, crate::USBError> {
    let send_buf: [u8; 8] = [
        address | device_type::SMD,
        device_type::MASTER,
        mode::ANGLE,
        port,
        angle,
        0,
        0,
        0,
    ];
    handle
        .write_bulk(&send_buf, Duration::from_millis(5000))
        .unwrap();
    return receive_status(handle, address);
}

/// Sends a command to set angles on the specified SMD device.
/// ## Example
/// Sample code to set angles of motors connected to the SMD at address 0x20, port 0 to 30 degrees and port 1 to 60 degrees.
/// ```rust
/// use motor_lib::{USBHandle, USBError, smd};
/// fn main() -> Result<(), USBError> {
///     let handle = USBHandle;
///     smd::send_angles(&handle, 0x20, 30, 60)?;
///     Ok(())
/// }
/// ```
pub fn send_angles(
    handle: &impl usb::USBHandleTrait,
    address: u8,
    angle_0: u8,
    angle_1: u8,
) -> Result<SmdStatus, crate::USBError> {
    let send_buf: [u8; 8] = [
        address | device_type::SD,
        device_type::MASTER,
        mode::ANGLES,
        0,
        angle_0,
        angle_1,
        0,
        0,
    ];
    handle
        .write_bulk(&send_buf, Duration::from_millis(5000))
        .unwrap();
    return receive_status(handle, address);
}

/// Receive a data from the specified SMD device.
/// ## Example
/// Sample code to retrieve status data from the SMD at address 0x20.
/// ```rust
/// use motor_lib::{USBHandle, USBError, smd};
/// fn main() -> Result<(), USBError> {
///     let handle = USBHandle;
///     let status = smd::receive_status(&handle, 0x20)?;
///     println!("{:?}", status);
///     Ok(())
/// }
/// ```
pub fn receive_status(
    handle: &impl usb::USBHandleTrait,
    address: u8,
) -> Result<SmdStatus, crate::USBError> {
    let mut receive_buf = [0; 8];
    loop {
        handle
            .read_bulk(&mut receive_buf, Duration::from_millis(5000))
            .unwrap();
        if (address | device_type::SD) == receive_buf[0] {
            return Ok(SmdStatus {
                address: receive_buf[0],
                semi_id: receive_buf[1],
                angle: ((receive_buf[2] as i16) << 8 | (receive_buf[3] as i16)),
                speed: ((receive_buf[4] as i16) << 8 | (receive_buf[5] as i16)),
            });
        }
    }
}

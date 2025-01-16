use crate::HandleTrait;
use advanced_pid::{prelude::*, VelPid};
use std::time::Duration;

pub mod mode {
    pub const INIT: u8 = 0;
    pub const STATUS: u8 = 1;
    pub const CURRENT: u8 = 2;
    pub const VELOCITY: u8 = 3;
    pub const ANGLE: u8 = 4;
}

#[derive(Debug)]
pub struct BlMdStatus {
    pub std_id: u16,
    pub angle: i16,
    pub speed: i16,
    pub current: i16,
}

/// Sends a velocity command to the specified device.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the HandleTrait.
/// * `pid` - A mutable reference to a VelPid object.
/// * `controller_id` - The ID of the controller.
/// * `velocity` - The desired velocity.
///
/// # Returns
///
/// A result containing the status of the device or an Error.
pub fn send_velocity(
    handle: &impl HandleTrait,
    pid: &mut VelPid,
    controller_id: u8,
    velocity: i16,
) -> Result<BlMdStatus, crate::Error> {
    let status = match receive_status(handle, controller_id) {
        Ok(s) => s,
        Err(e) => return Err(e),
    };
    let actual = status.speed;
    let output = pid.update(velocity as f32, actual as f32, 0.3) as i16; // 制御周期100Hz
    let return_status = send_current(handle, controller_id, output);
    if cfg!(test) {
        println!("{},{},{}", actual, output, status.speed);
    }
    return return_status;
}

pub fn send_current(
    handle: &impl HandleTrait,
    controller_id: u8,
    current: i16,
) -> Result<BlMdStatus, crate::Error> {
    let send_buf: [u8; 8] = [
        0x30,
        0,
        controller_id,
        ((current >> 8) & 0xff) as u8,
        (current & 0xff) as u8,
        0,
        0,
        0,
    ];
    handle.write_bulk(&send_buf, Duration::from_millis(5000))?;
    return receive_status(handle, controller_id);
}

pub fn receive_status(
    handle: &impl HandleTrait,
    controller_id: u8,
) -> Result<BlMdStatus, crate::Error> {
    let mut receive_buf = [0; 8];
    loop {
        match handle.read_bulk(&mut receive_buf, Duration::from_millis(5000)) {
            Ok(_) => {}
            Err(e) => return Err(e),
        }
        let received_stdid = (receive_buf[0] as u16) << 8 | (receive_buf[1] as u16);
        if received_stdid == (0x200 + (controller_id as u16)) {
            return Ok(BlMdStatus {
                std_id: received_stdid,
                angle: ((receive_buf[2] as i16) << 8 | (receive_buf[3] as i16)),
                speed: ((receive_buf[4] as i16) << 8 | (receive_buf[5] as i16)),
                current: ((receive_buf[6] as i16) << 8 | (receive_buf[7] as i16)),
            });
        }
    }
}

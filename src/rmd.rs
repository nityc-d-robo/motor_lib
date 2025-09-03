use std::{ops::Index, time::Duration};

use crate::{device_type, HandleTrait, Error};

pub mod mode {
    pub const CURRENT: u8 = 0;
    pub const SPEED: u8 = 1;
    pub const ANGLE: u8 = 2;
}

#[derive(Debug)]
pub struct RmStatus {
    pub id: u8,
    pub rotor_angle: i16,
    pub rotational_speed: i16,
    pub torque_current: i16,
}

/// Sends a command to set the motor current for a single C620 controller.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the USBHandleTrait.
/// * `id` - The ID of the C620 controller (1-8).
/// * `current` - The current value to set. Range is -20.0 to 20.0 (Amps).
///
/// # Returns
///
/// A result containing () or an Error.
/// 
/// # Example
/// 
/// samplecode
/// ```rust
/// use motorlib
/// ```

pub fn send_current( 
    handle: &impl HandleTrait, 
    id: u8, 
    current: f32, 
) { 
    if id < 1 || id > 8 { 
        return; 
    } 

    let (identifier, data_index) = if id <= 4 { 
        (0x200, (id - 1) * 2) 
    } else { 
        (0x1FF, (id - 5) * 2) 
    }; 

    // Map current from -20.0~20.0A to -16384~16384 (i16) 
    let current_i16 = (current * 819.2).clamp(-16384.0, 16384.0) as i16; 

    let mut send_buf: [u8; 10] = [0; 10]; 

    // [0], [1] にCAN IDを格納
    send_buf[0] = ((identifier >> 8) & 0xFF) as u8; 
    send_buf[1] = (identifier & 0xFF) as u8; 

    let payload_index = 2 + data_index as usize;
    send_buf[payload_index] = (current_i16 >> 8) as u8;
    send_buf[payload_index + 1] = current_i16 as u8;

    let _ = handle.write_bulk(&send_buf, Duration::from_millis(5000)); 
}

/// Receives feedback data from a specified C620 controller.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the USBHandleTrait.
/// * `id` - The ID of the C620 controller (1-8).
///
/// # Returns
///
/// A result containing the status of the C620 controller or an Error.
pub fn receive_status(
    handle: &impl HandleTrait,
    id: u8,
) -> Result<RmStatus, Error> {
    let mut receive_buf = [0; 8];
    let expected_identifier = 0x200 + id as u16;

    loop {
        handle.read_bulk(&mut receive_buf, Duration::from_millis(5000))?;
        let identifier = ((receive_buf[1] as u16) << 8) | (receive_buf[0] as u16);

        if identifier == expected_identifier {
            return Ok(RmStatus {
                id: id,
                rotor_angle: ((receive_buf[2] as i16) << 8 | (receive_buf[3] as i16)),
                rotational_speed: ((receive_buf[4] as i16) << 8 | (receive_buf[5] as i16)),
                torque_current: ((receive_buf[6] as i16) << 8 | (receive_buf[7] as i16))
            });
        }
    }
}
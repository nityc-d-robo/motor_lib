use std::time::Duration;

pub mod blmd;
pub mod md;
pub mod sd;
pub mod smd;
pub mod sr;
pub mod usb;

pub mod device_type {
    pub const MD: u8 = 0x00;
    pub const SD: u8 = 0x10;
    pub const SMD: u8 = 0x20;
    pub const BLMD: u8 = 0x30;
    pub const SR: u8 = 0x40;
    pub const SM: u8 = 0x50;
    pub const MASTER: u8 = 0x60;
    pub const EMMERGENCY: u8 = 0xf0;
}

pub struct USBHandle;
#[derive(Debug)]
pub enum USBError {
    RUsbError(rusb::Error),
}

/// Sends an emergency signal to the drobo CAN device (for example, MD, SD, etc.)   
/// It's not possible to confirm whether the signal was sent properly, and this function always returns nothing.
/// ## Example
/// ```rust
/// use motor_lib::{USBHandle, send_emergency};
/// fn main() {
///     let handle = USBHandle;
///     send_emergency(&handle);
/// }
/// ```
pub fn send_emergency(handle: &impl usb::USBHandleTrait) {
    let send_buf: [u8; 8] = [
        device_type::EMMERGENCY,
        device_type::MASTER,
        0,
        0,
        0,
        0,
        0,
        0,
    ];
    handle
        .write_bulk(&send_buf, Duration::from_millis(5000))
        .unwrap_or(8);
}

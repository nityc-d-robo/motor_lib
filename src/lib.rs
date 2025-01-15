//! This library provides an interface for controlling various motor devices via USB.
use std::{fmt, time::Duration};

pub mod blmd;
pub mod device_type;
mod implements;
pub mod md;
pub mod sd;
pub mod smd;
pub mod sr;
pub use implements::grpc;
pub use implements::grpc::GrpcHandle;
pub use implements::usb;
pub use implements::usb::USBHandle;

#[derive(Debug)]
pub enum Error {
    RUsbError(rusb::Error),
    GrpcError(tonic::Status),
}

impl fmt::Display for crate::Error {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            crate::Error::RUsbError(e) => write!(f, "RUsbError: {}", e),
            crate::Error::GrpcError(e) => write!(f, "gRPCError: {}", e),
        }
    }
}

/// A trait defining the interface for USB handle operations.
pub trait HandleTrait {
    fn read_bulk(&self, data: &mut [u8], timeout: Duration) -> Result<usize, Error>;
    fn write_bulk(&self, data: &[u8], timeout: Duration) -> Result<usize, Error>;
}

/// Sends an emergency signal to the drobo CAN device (for example, MD, SD, etc.)   
/// It's not possible to confirm whether the signal was sent properly, and this function always returns nothing.
///
/// # Arguments
///
/// * `handle` - A reference to an object implementing the USBHandleTrait.
///
/// # Returns
///
/// A result containing the number of bytes written or an Error.
///
/// # Example
/// ```rust
/// use motor_lib::{USBHandle, Error, send_emergency};
/// fn main() -> Result<(), Error> {
///     let handle = USBHandle::new(0x483, 0x5740, 1);
///     send_emergency(&handle)?;
///     Ok(())
/// }
/// ```
pub fn send_emergency(handle: &impl HandleTrait) -> Result<usize, Error> {
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
    handle.write_bulk(&send_buf, Duration::from_millis(5000))
}

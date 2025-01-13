//! Implementation of an abstraction layer for accessing USB devices.

use crate::{USBError, USBHandle};
use rusb::constants::{LIBUSB_ENDPOINT_IN, LIBUSB_ENDPOINT_OUT};
use rusb::open_device_with_vid_pid;
use std::{fmt, time};

mod end_point {
    pub static EP1: u8 = 1;
}

impl fmt::Display for USBError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            USBError::RUsbError(e) => write!(f, "RUsbError: {}", e),
        }
    }
}
impl From<rusb::Error> for USBError {
    fn from(error: rusb::Error) -> Self {
        USBError::RUsbError(error)
    }
}

/// A trait defining the interface for USB handle operations.
pub trait USBHandleTrait {
    /// Reads data from a USB device using bulk transfer.
    ///
    /// # Arguments
    ///
    /// * `data` - A mutable slice to store the data read from the device.
    /// * `timeout` - The duration to wait for the operation to complete.
    ///
    /// # Returns
    ///
    /// A result containing the number of bytes read or a USBError.
    fn read_bulk(&self, data: &mut [u8], timeout: time::Duration) -> Result<usize, USBError>;

    /// Writes data to a USB device using bulk transfer.
    ///
    /// # Arguments
    ///
    /// * `data` - A slice containing the data to write to the device.
    /// * `timeout` - The duration to wait for the operation to complete.
    ///
    /// # Returns
    ///
    /// A result containing the number of bytes written or a USBError.
    fn write_bulk(&self, data: &[u8], timeout: time::Duration) -> Result<usize, USBError>;
}

impl USBHandle {
    pub fn new(vendor_id: u16, product_id: u16, b_interface_number: u8) -> Self {
        let handle = open_device_with_vid_pid(vendor_id, product_id).unwrap();
        handle.set_auto_detach_kernel_driver(true).unwrap_or(());
        handle.claim_interface(b_interface_number).unwrap();
        Self { handle }
    }
}

/// Implementation of the USBHandleTrait for the USBHandle struct.
impl USBHandleTrait for USBHandle {
    fn read_bulk(&self, data: &mut [u8], timeout: time::Duration) -> Result<usize, USBError> {
        let result = self
            .handle
            .read_bulk(LIBUSB_ENDPOINT_IN | end_point::EP1, data, timeout)
            .map_err(USBError::from);
        result
    }

    fn write_bulk(&self, data: &[u8], timeout: time::Duration) -> Result<usize, USBError> {
        let result = self
            .handle
            .write_bulk(LIBUSB_ENDPOINT_OUT | end_point::EP1, data, timeout)
            .map_err(USBError::from);
        result
    }
}

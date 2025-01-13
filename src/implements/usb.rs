//! Implementation of an abstraction layer for accessing USB devices.

use crate::HandleTrait;
use rusb::constants::{LIBUSB_ENDPOINT_IN, LIBUSB_ENDPOINT_OUT};
use rusb::open_device_with_vid_pid;
use std::time;

mod end_point {
    pub static EP1: u8 = 1;
}

/// A handle to read and write an USB device.
pub struct USBHandle {
    handle: rusb::DeviceHandle<rusb::GlobalContext>,
}

impl USBHandle {
    pub fn new(vendor_id: u16, product_id: u16, b_interface_number: u8) -> Self {
        let handle = open_device_with_vid_pid(vendor_id, product_id).unwrap();
        handle.set_auto_detach_kernel_driver(true).unwrap_or(());
        handle.claim_interface(b_interface_number).unwrap();
        Self { handle }
    }
}

impl HandleTrait for USBHandle {
    fn read_bulk(&self, data: &mut [u8], timeout: time::Duration) -> Result<usize, crate::Error> {
        let result = self
            .handle
            .read_bulk(LIBUSB_ENDPOINT_IN | end_point::EP1, data, timeout)
            .map_err(crate::Error::from);
        result
    }

    fn write_bulk(&self, data: &[u8], timeout: time::Duration) -> Result<usize, crate::Error> {
        let result = self
            .handle
            .write_bulk(LIBUSB_ENDPOINT_OUT | end_point::EP1, data, timeout)
            .map_err(crate::Error::from);
        result
    }
}

impl From<rusb::Error> for crate::Error {
    fn from(error: rusb::Error) -> Self {
        crate::Error::RUsbError(error)
    }
}

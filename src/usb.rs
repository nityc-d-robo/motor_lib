//! Implementation of an abstraction layer for accessing USB devices.

use rusb::constants::{LIBUSB_ENDPOINT_IN, LIBUSB_ENDPOINT_OUT};
use rusb::{DeviceHandle, GlobalContext};
use std::{fmt, sync::LazyLock, time};

const VENDOR_ID: u16 = 0x483;
const PRODUCT_ID: u16 = 0x5740;
const B_INTERFACE_NUMBER: u8 = 1;
static HANDLE: LazyLock<DeviceHandle<GlobalContext>> = LazyLock::new(|| {
    use rusb::open_device_with_vid_pid;
    let handle = open_device_with_vid_pid(VENDOR_ID, PRODUCT_ID).unwrap();
    handle.set_auto_detach_kernel_driver(true).unwrap_or(());
    handle.claim_interface(B_INTERFACE_NUMBER).unwrap();
    handle
});

mod end_point {
    pub static EP1: u8 = 1;
}

impl fmt::Display for crate::USBError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            crate::USBError::RUsbError(e) => write!(f, "RUsbError: {}", e),
        }
    }
}
impl From<rusb::Error> for crate::USBError {
    fn from(error: rusb::Error) -> Self {
        crate::USBError::RUsbError(error)
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
    fn read_bulk(&self, data: &mut [u8], timeout: time::Duration)
        -> Result<usize, crate::USBError>;

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
    fn write_bulk(&self, data: &[u8], timeout: time::Duration) -> Result<usize, crate::USBError>;
}

impl crate::USBHandle {
    pub fn new() -> Self {
        Self{}
    }
}

/// Implementation of the USBHandleTrait for the USBHandle struct.
impl USBHandleTrait for crate::USBHandle {
    fn read_bulk(
        &self,
        data: &mut [u8],
        timeout: time::Duration,
    ) -> Result<usize, crate::USBError> {
        let result = HANDLE
            .read_bulk(LIBUSB_ENDPOINT_IN | end_point::EP1, data, timeout)
            .map_err(crate::USBError::from);
        result
    }

    fn write_bulk(&self, data: &[u8], timeout: time::Duration) -> Result<usize, crate::USBError> {
        let result = HANDLE
            .write_bulk(LIBUSB_ENDPOINT_OUT | end_point::EP1, data, timeout)
            .map_err(crate::USBError::from);
        result
    }
}

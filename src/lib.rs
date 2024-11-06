use std::time::Duration;
use rusb::{open_device_with_vid_pid, constants::LIBUSB_ENDPOINT_OUT};
pub use rusb::{DeviceHandle, GlobalContext, Error};

pub mod md;
pub mod sd;
pub mod smd;
pub mod blmd;
pub mod sr;

#[allow(non_snake_case)]
pub mod IdType {
    pub const MD: u8 = 0x00;
    pub const SD: u8 = 0x10;
    pub const SMD: u8 = 0x20;
    pub const BLMD: u8 = 0x30;
    pub const SR: u8 = 0x40;
    pub const SM: u8 = 0x50;
    pub const MASTER: u8 = 0x60;
    pub const EMMERGENCY: u8 = 0xf0;
}

#[allow(non_snake_case)]
pub mod EndPont {
    pub static EP1: u8 = 1;
}

pub fn init_usb_handle(vendor_id: u16, product_id: u16, b_interface_number: u8) -> Result<DeviceHandle<GlobalContext>, rusb::Error>{
    let handle = loop {
        match open_device_with_vid_pid(vendor_id, product_id) {
            Some(t) => break t,
            None => {
                eprintln!("usb_can_hardware not detected.");
                std::thread::sleep(core::time::Duration::from_secs(1));
            }
        }
    };
    handle.set_auto_detach_kernel_driver(true).unwrap_or(());
    handle.claim_interface(b_interface_number)?;
    return Ok(handle);
}

pub fn send_emergency(handle_: &DeviceHandle<GlobalContext>){
    let send_buf: [u8; 8] = [
        IdType::EMMERGENCY,
        IdType::MASTER,
        0,
        0,
        0,
        0,
        0,
        0
    ];
    handle_.write_bulk(LIBUSB_ENDPOINT_OUT | EndPont::EP1, &send_buf, Duration::from_millis(5000)).unwrap();
}

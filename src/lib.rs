use std::time::Duration;
use rusb::{open_device_with_vid_pid, DeviceHandle, GlobalContext, constants::LIBUSB_ENDPOINT_OUT};

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

pub fn init_usb_handle(vendor_id: u16, product_id: u16, b_interface_number: u8) -> DeviceHandle<GlobalContext>{
    let handle = open_device_with_vid_pid(vendor_id, product_id).unwrap();
    handle.set_auto_detach_kernel_driver(true).unwrap_or(());
    handle.claim_interface(b_interface_number).unwrap();
    return handle
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

#[cfg(test)]
mod tests {
    use super::*;

    use advanced_pid::{prelude::*, PidConfig, VelPid};
    use std::{time::Duration, thread::sleep};

    #[test]
    fn motor_rotation() {
        let handle = init_usb_handle(0x483, 0x5740, 1);
        for i in (0..=1024).step_by(1){
            let return_status = md::send_pwm(&handle, 0x00, i);
            println!("power: {i}, {:?}", return_status);
            sleep(Duration::from_millis(10));
        }
        send_emergency(&handle);
    }
    #[test]
    fn speed_pid() {
        let handle = init_usb_handle(0x483, 0x5740, 1);
        let config = PidConfig::new(1.0, 0.1, 0.1).with_limits(-16384.0, 16384.0);
        let mut pid = VelPid::new(config);
        for _i in (0..=100).step_by(1){
            blmd::send_velocity(&handle, &mut pid, 2, 500);
            sleep(Duration::from_millis(10));
        }
        send_emergency(&handle);
    }
}

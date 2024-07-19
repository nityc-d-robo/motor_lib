use rusb::{open_device_with_vid_pid, DeviceHandle, GlobalContext};

pub mod md;
pub mod blmd;

#[allow(non_snake_case)]
pub mod IdType {
    pub static MD: u8 = 0x00;
    pub static SD: u8 = 0x10;
    pub static SMD: u8 = 0x20;
    pub static BLMD: u8 = 0x30;
    pub static SR: u8 = 0x40;
    pub static SM: u8 = 0x50;
    pub static MASTER: u8 = 0x60;
    pub static EMMERGENCY: u8 = 0xf0;
}

#[allow(non_snake_case)]
pub mod EndPont {
    pub static EP1: u8 = 1;
}

pub fn init_usb_handle(vendor_id: u16, product_id: u16, b_interface_number: u8) -> DeviceHandle<GlobalContext>{
    let handle = open_device_with_vid_pid(vendor_id, product_id).unwrap();
    handle.set_auto_detach_kernel_driver(true).unwrap();
    handle.claim_interface(b_interface_number).unwrap();
    return handle
}

#[cfg(test)]
mod tests {
    use super::*;

    use blmd::{send_velocity, send_current};
    use advanced_pid::{prelude::*, PidConfig, VelPid};
    use std::{time::Duration, thread::sleep};

    #[test]
    fn motor_rotation() {
        let handle = init_usb_handle(0x483, 0x5740, 0);
        for i in (0..=16384).step_by(100){
            let return_status = send_current(&handle, 2, i);
            println!("power: {i}, {:?}", return_status);
            sleep(Duration::from_millis(100));
        }
    }
    #[test]
    fn speed_pid() {
        let handle = init_usb_handle(0x483, 0x5740, 0);
        let config = PidConfig::new(1.0, 0.1, 0.1).with_limits(-16384.0, 16384.0);
        let mut pid = VelPid::new(config);
        for _i in (0..=100).step_by(1){
            send_velocity(&handle, &mut pid, 2, 500);
            sleep(Duration::from_millis(10));
        }
    }
}

use rusb::{open_device_with_vid_pid, DeviceHandle, GlobalContext};

pub mod blmd;

pub fn init_usb_handle(vendor_id: u16, product_id: u16, b_interface_number: u8) -> DeviceHandle<GlobalContext>{
    let handle = open_device_with_vid_pid(vendor_id, product_id).unwrap();
    handle.set_auto_detach_kernel_driver(true).unwrap();
    handle.claim_interface(b_interface_number).unwrap();
    return handle
}

#[cfg(test)]
mod tests {
    use super::*;

    use blmd::send_current;
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
}

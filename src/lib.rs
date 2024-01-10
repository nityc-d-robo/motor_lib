use rusb::{open_device_with_vid_pid, DeviceHandle, GlobalContext};

pub mod md{
    use std::time::Duration;
    use rusb::{DeviceHandle, GlobalContext, constants::{LIBUSB_ENDPOINT_IN, LIBUSB_ENDPOINT_OUT}};

    #[derive(Debug)]
    pub struct MdStatus{
        pub std_id: u16,
        pub angle: i16,
        pub speed: i16,
        pub current: i16,
    }

    pub fn send_current(handle_: &DeviceHandle<GlobalContext>, end_point_: u8, std_id_: u16, controller_id_: u8, current_: i16) -> MdStatus{
        let send_buf: [u8; 8] = [
            ((std_id_ >> 8) & 0xff) as u8,
            (std_id_ & 0xff) as u8,
            controller_id_,
            ((current_ >> 8) & 0xff) as u8,
            (current_ & 0xff) as u8,
            0,
            0,
            0
        ];
        handle_.write_bulk(LIBUSB_ENDPOINT_OUT | end_point_, &send_buf, Duration::from_millis(5000)).unwrap();
        
        let mut receive_buf:[u8; 8] = [0; 8];
        loop {
            handle_.read_bulk(LIBUSB_ENDPOINT_IN | end_point_, &mut receive_buf, Duration::from_millis(5000)).unwrap();
            let received_stdid = (receive_buf[0] as u16) << 8 | (receive_buf[1] as u16);
            if received_stdid == (std_id_ + (controller_id_ as u16)){
                return MdStatus{
                    std_id: received_stdid,
                    angle: ((receive_buf[2] as i16) << 8 | (receive_buf[3] as i16)),
                    speed: ((receive_buf[4] as i16) << 8 | (receive_buf[5] as i16)),
                    current: ((receive_buf[6] as i16) << 8 | (receive_buf[7] as i16)),
                }
            }
        }
    }
}

pub fn init_usb_handle(vendor_id: u16, product_id: u16, b_interface_number: u8) -> DeviceHandle<GlobalContext>{
    let mut handle = open_device_with_vid_pid(vendor_id, product_id).unwrap();
    handle.set_auto_detach_kernel_driver(true).unwrap();
    handle.claim_interface(b_interface_number).unwrap();
    return handle
}

#[cfg(test)]
mod tests {
    use super::*;

    use crate::md::send_current;
    use std::{time::Duration, thread::sleep};

    #[test]
    fn it_works() {
        let handle = init_usb_handle(0x483, 0x5740, 0);
        for i in (0..=16384).step_by(10){
            let return_status = send_current(&handle, 1, 0x200, 1, i);
            println!("power: {i}, {:?}", return_status);
            sleep(Duration::from_millis(100));
        }
        for i in (-16384..=16384).step_by(10).rev(){
            let return_status = send_current(&handle, 1, 0x200, 1, i);
            println!("power: {i}, {:?}", return_status);
            sleep(Duration::from_millis(100));
        }
        for i in (-16384..=0).step_by(10).rev(){
            let return_status = send_current(&handle, 1, 0x200, 1, i);
            println!("power: {i}, {:?}", return_status);
            sleep(Duration::from_millis(100));
        }
    }
}

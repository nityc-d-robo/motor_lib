use std::time::Duration;
use rusb::{DeviceHandle, GlobalContext, constants::{LIBUSB_ENDPOINT_IN, LIBUSB_ENDPOINT_OUT}};
use advanced_pid::{prelude::*, VelPid};

use crate::EndPont;

#[derive(Debug)]
pub struct BlMdStatus {
    pub std_id: u16,
    pub angle: i16,
    pub speed: i16,
    pub current: i16,
}

pub fn send_velocity(handle_: &DeviceHandle<GlobalContext>, pid_: &mut VelPid, controller_id_: u8, velocity_: i16) -> BlMdStatus{
    let actual = receive_status(handle_, controller_id_).speed;
    let output = pid_.update(velocity_ as f32, actual as f32, 0.3) as i16; // 制御周期100Hz
    let return_status = send_current(handle_, controller_id_, output);
    if cfg!(test){
        println!("{},{},{}", actual, output, return_status.speed);
    }
    return return_status;
}

pub fn send_current(handle_: &DeviceHandle<GlobalContext>, controller_id_: u8, current_: i16) -> BlMdStatus{
    let send_buf: [u8; 8] = [
        0x30,
        0,
        controller_id_,
        ((current_ >> 8) & 0xff) as u8,
        (current_ & 0xff) as u8,
        0,
        0,
        0
    ];
    handle_.write_bulk(LIBUSB_ENDPOINT_OUT | EndPont::EP1, &send_buf, Duration::from_millis(5000)).unwrap();
    return receive_status(handle_, controller_id_)
}

pub fn receive_status(handle_: &DeviceHandle<GlobalContext>, controller_id_: u8) -> BlMdStatus{
    let mut receive_buf = [0;8];
    loop {
        handle_.read_bulk(LIBUSB_ENDPOINT_IN | EndPont::EP1, &mut receive_buf, Duration::from_millis(5000)).unwrap();
        let received_stdid = (receive_buf[0] as u16) << 8 | (receive_buf[1] as u16);
        if received_stdid == (0x200 + (controller_id_ as u16)){
            return BlMdStatus{
                std_id: received_stdid,
                angle: ((receive_buf[2] as i16) << 8 | (receive_buf[3] as i16)),
                speed: ((receive_buf[4] as i16) << 8 | (receive_buf[5] as i16)),
                current: ((receive_buf[6] as i16) << 8 | (receive_buf[7] as i16)),
            }
        }
    }
}

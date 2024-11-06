use std::time::Duration;
use rusb::{DeviceHandle, GlobalContext, constants::{LIBUSB_ENDPOINT_IN, LIBUSB_ENDPOINT_OUT}, Error};
use advanced_pid::{prelude::*, VelPid};

use crate::EndPont;

#[allow(non_snake_case)]
pub mod Mode {
    pub const INIT: u8 = 0;
    pub const STATUS: u8 = 1;
    pub const CURRENT: u8 = 2;
    pub const VELOCITY: u8 = 3;
    pub const ANGLE: u8 = 4;
}

#[derive(Debug)]
pub struct BlMdStatus {
    pub std_id: u16,
    pub angle: i16,
    pub speed: i16,
    pub current: i16,
}

pub fn send_velocity(handle_: &DeviceHandle<GlobalContext>, pid_: &mut VelPid, controller_id_: u8, velocity_: i16) -> Result<BlMdStatus, Error>{
    let status = match receive_status(handle_, controller_id_) {
        Ok(s) => s,
        Err(e) => return Err(e),
    };
    let actual = status.speed;
    let output = pid_.update(velocity_ as f32, actual as f32, 0.3) as i16; // 制御周期100Hz
    let return_status = send_current(handle_, controller_id_, output);
    if cfg!(test){
        println!("{},{},{}", actual, output, status.speed);
    }
    return return_status;
}

pub fn send_current(handle_: &DeviceHandle<GlobalContext>, controller_id_: u8, current_: i16) -> Result<BlMdStatus, Error>{
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

pub fn receive_status(handle_: &DeviceHandle<GlobalContext>, controller_id_: u8) -> Result<BlMdStatus, Error>{
    let mut receive_buf = [0;8];
    loop {
        match handle_.read_bulk(LIBUSB_ENDPOINT_IN | EndPont::EP1, &mut receive_buf, Duration::from_millis(5000)){
            Ok(_) => {},
            Err(e) => return Err(e),
        }
        let received_stdid = (receive_buf[0] as u16) << 8 | (receive_buf[1] as u16);
        if received_stdid == (0x200 + (controller_id_ as u16)){
            return Ok(BlMdStatus{
                std_id: received_stdid,
                angle: ((receive_buf[2] as i16) << 8 | (receive_buf[3] as i16)),
                speed: ((receive_buf[4] as i16) << 8 | (receive_buf[5] as i16)),
                current: ((receive_buf[6] as i16) << 8 | (receive_buf[7] as i16)),
            })
        }
    }
}

#[cfg(test)]
mod tests {
    use crate::{init_usb_handle, send_emergency, blmd};

    use advanced_pid::{prelude::*, PidConfig, VelPid};
    use std::{time::Duration, thread::sleep};

    #[test]
    fn motor_rotation() {
        let handle = init_usb_handle(0x483, 0x5740, 1).unwrap();
        // エラーハンドリング
        let return_status = 
            loop {
                match blmd::send_current(&handle, 1, 1000) {
                    Ok(t) => {
                        // エラー処理解除
                        break t
                    },
                    Err(_) => {
                        // 何らかのエラー処理
                    }
                };
            };
        println!("power: 1000, {:?}", return_status.current);
        sleep(Duration::from_millis(10));
        send_emergency(&handle);
    }
    
    #[test]
    fn speed_pid() {
        let handle = init_usb_handle(0x483, 0x5740, 1).unwrap();
        let config = PidConfig::new(1.0, 0.1, 0.1).with_limits(-16384.0, 16384.0);
        let mut pid = VelPid::new(config);
        for _i in (0..=100).step_by(1){
            blmd::send_velocity(&handle, &mut pid, 2, 500).unwrap();
            sleep(Duration::from_millis(10));
        }
        send_emergency(&handle);
    }
}

use motor_lib::{md, auto_detect};

fn main() -> Result<(), motor_lib::Error> {
    let handle = auto_detect().expect("Device not found");
    md::send_pwm(handle.as_ref(), 0x01, 1000);
    Ok(())
}

use motor_lib::{md, GrpcHandle};

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let handle = GrpcHandle::new("http://127.0.0.1:50051");
    md::send_pwm(&handle, 0x01, 1000).unwrap();
    Ok(())
}

use motor_lib::{blmd::receive_status, md, GrpcHandle, rmd};

fn main() -> Result<(), motor_lib::Error> {
    let handle = GrpcHandle::new("http://127.0.0.1:50051");
    let current: [f32;4] = [
        1.0,
        1.0,
        1.0,
        1.0,
    ];
    let _ = rmd::send_current(&handle, 0x200, current);
    
    Ok(())
}

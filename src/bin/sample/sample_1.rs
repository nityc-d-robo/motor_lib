use motor_lib::{grpc::pb, md, GrpcHandle};
use std::cell::RefCell;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let tokio_context = tokio::runtime::Runtime::new().unwrap();
    let client = tokio_context.block_on(async {
        pb::usb_can_client::UsbCanClient::connect("http://127.0.0.1:50051")
            .await
            .unwrap()
    });
    let handle = GrpcHandle::new(tokio_context, RefCell::new(client));
    md::send_pwm(&handle, 0x00, 1000).unwrap();
    Ok(())
}

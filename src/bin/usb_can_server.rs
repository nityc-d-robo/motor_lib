pub mod pb {
    tonic::include_proto!("motor_lib");
}

use rusb::constants::{LIBUSB_ENDPOINT_IN, LIBUSB_ENDPOINT_OUT};
use std::{env, time};
use tonic::transport::Server;

const EP1: u8 = 1; // usb endpoint. use for implement usb_can_server::UsbCan trait.
const TIMEOUT: time::Duration = time::Duration::from_millis(5000);

#[derive(Debug)]
pub struct UsbCanServer {
    handle: rusb::DeviceHandle<rusb::GlobalContext>,
}

#[tonic::async_trait]
impl pb::usb_can_server::UsbCan for UsbCanServer {
    async fn read(
        &self,
        _request: tonic::Request<pb::ReadRequest>,
    ) -> Result<tonic::Response<pb::ReadResponse>, tonic::Status> {
        let mut recv_buf = vec![0; _request.into_inner().size as usize];
        self.handle
            .read_bulk(LIBUSB_ENDPOINT_IN | EP1, &mut recv_buf, TIMEOUT)
            .unwrap_or_default();
        Ok(tonic::Response::new(pb::ReadResponse {
            recv_buf: recv_buf,
        }))
    }
    async fn write(
        &self,
        _request: tonic::Request<pb::WriteRequest>,
    ) -> Result<tonic::Response<pb::WriteResponse>, tonic::Status> {
        let send_buf = _request.into_inner().send_buf;
        let option = self
            .handle
            .write_bulk(LIBUSB_ENDPOINT_OUT | EP1, &send_buf, TIMEOUT)
            .ok();
        Ok(tonic::Response::new(pb::WriteResponse {
            size: option.and_then(|size| size.try_into().ok()).unwrap_or(-1),
        }))
    }
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    const VENDOR_ID: u16 = 0x483;
    const PRODUCT_ID: u16 = 0x5740;
    const B_INTERFACE_NUMBER: u8 = 1;
    let handle = rusb::open_device_with_vid_pid(VENDOR_ID, PRODUCT_ID).unwrap();
    handle.set_auto_detach_kernel_driver(true).unwrap_or(());
    handle.claim_interface(B_INTERFACE_NUMBER).unwrap();

    let server = UsbCanServer { handle };

    let args: Vec<String> = env::args().collect();
    const DEFAULT_ADDRESS: &str = "127.0.0.1:50051";
    let socket_address = match args.get(1) {
        Some(str) => str.parse().unwrap(), // コマンドライン引数にヘンな値が入ってたら落とす
        None => DEFAULT_ADDRESS.parse().unwrap(), // プログラマの責任で安全にunwrapできる
    };

    println!("Server listening on {}", socket_address);

    Server::builder()
        .add_service(pb::usb_can_server::UsbCanServer::new(server))
        .serve(socket_address)
        .await
        .unwrap();
    Ok(())
}

pub mod pb {
    tonic::include_proto!("motor_lib");
}

use rusb::constants::{LIBUSB_ENDPOINT_IN, LIBUSB_ENDPOINT_OUT};
use rusb::{Context, Device, HotplugBuilder, UsbContext};
use std::{
    env,
    sync::{Arc, Mutex},
    time,
};
use tonic::transport::Server;

const VENDOR_ID: u16 = 0x483;
const PRODUCT_ID: u16 = 0x5740;
const B_INTERFACE_NUMBER: u8 = 1;

const EP1: u8 = 1; // usb endpoint. use for implement usb_can_server::UsbCan trait.
const TIMEOUT: time::Duration = time::Duration::from_millis(5000);

struct HotPlugHandler {
    handle: Arc<Mutex<Option<rusb::DeviceHandle<rusb::GlobalContext>>>>,
}

impl HotPlugHandler {
    fn new(handle: Arc<Mutex<Option<rusb::DeviceHandle<rusb::GlobalContext>>>>) -> HotPlugHandler {
        HotPlugHandler { handle }
    }
}

impl<T: UsbContext> rusb::Hotplug<T> for HotPlugHandler {
    fn device_arrived(&mut self, _: Device<T>) {
        let mut locked_handle = self.handle.lock().unwrap();
        if (locked_handle).is_some() {
            let handle = rusb::open_device_with_vid_pid(VENDOR_ID, PRODUCT_ID).unwrap();
            handle.set_auto_detach_kernel_driver(true).unwrap_or(());
            handle.claim_interface(B_INTERFACE_NUMBER).unwrap();

            *locked_handle = Some(handle);
            println!("Device arrived");
        }
    }

    fn device_left(&mut self, _: Device<T>) {
        let mut locked_handle = self.handle.lock().unwrap();
        *locked_handle = None;
        println!("Device left");
    }
}

impl Drop for HotPlugHandler {
    fn drop(&mut self) {
        println!("HotPlugHandler dropped");
    }
}

#[derive(Debug)]
pub struct UsbCanServer {
    handle: Arc<Mutex<Option<rusb::DeviceHandle<rusb::GlobalContext>>>>,
}

impl UsbCanServer {
    fn new(handle: Arc<Mutex<Option<rusb::DeviceHandle<rusb::GlobalContext>>>>) -> UsbCanServer {
        UsbCanServer { handle }
    }
}

#[tonic::async_trait]
impl pb::usb_can_server::UsbCan for UsbCanServer {
    async fn read(
        &self,
        _request: tonic::Request<pb::ReadRequest>,
    ) -> Result<tonic::Response<pb::ReadResponse>, tonic::Status> {
        let mut recv_buf = vec![0; _request.into_inner().size as usize];
        if let Some(ref handle) = *self.handle.lock().unwrap() {
            handle
                .read_bulk(LIBUSB_ENDPOINT_IN | EP1, &mut recv_buf, TIMEOUT)
                .unwrap_or_default();
        }
        Ok(tonic::Response::new(pb::ReadResponse {
            recv_buf: recv_buf,
        }))
    }
    async fn write(
        &self,
        _request: tonic::Request<pb::WriteRequest>,
    ) -> Result<tonic::Response<pb::WriteResponse>, tonic::Status> {
        let send_buf = _request.into_inner().send_buf;
        let option = match *self.handle.lock().unwrap() {
            Some(ref handle) => handle
                .write_bulk(LIBUSB_ENDPOINT_OUT | EP1, &send_buf, TIMEOUT)
                .ok(),
            None => None,
        };
        Ok(tonic::Response::new(pb::WriteResponse {
            size: option.and_then(|size| size.try_into().ok()).unwrap_or(-1),
        }))
    }
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let handle = Arc::new(Mutex::new(Some(
        rusb::open_device_with_vid_pid(VENDOR_ID, PRODUCT_ID).unwrap(),
    )));
    let locked_handle = handle.lock().unwrap();
    if let Some(ref device_handle) = *locked_handle {
        device_handle
            .set_auto_detach_kernel_driver(true)
            .unwrap_or(());
        device_handle.claim_interface(B_INTERFACE_NUMBER).unwrap();
    }

    if rusb::has_hotplug() {
        let context = Context::new()?;
        let _: Option<rusb::Registration<Context>> = Some(
            HotplugBuilder::new()
                .enumerate(true)
                .vendor_id(VENDOR_ID)
                .product_id(PRODUCT_ID)
                .register(&context, Box::new(HotPlugHandler::new(Arc::clone(&handle))))?,
        );
        tokio::task::spawn_blocking(move || {
            loop {
                context.handle_events(None).unwrap();
            }
        });
    }

    let server = UsbCanServer::new(Arc::clone(&handle));

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

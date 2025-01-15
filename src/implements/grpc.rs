//! Implementation of gRPC client for USB communication.

use crate::HandleTrait;
use std::{cell::RefCell, time};
pub mod pb {
    tonic::include_proto!("motor_lib");
}

use pb::WriteRequest;

/// A handle to read and write an gRPC device.
pub struct GrpcHandle {
    tokio_runtime: tokio::runtime::Runtime,
    client: RefCell<pb::usb_can_client::UsbCanClient<tonic::transport::Channel>>,
}

impl GrpcHandle {
    pub fn new(url: &str) -> Self {
        let tokio_runtime = tokio::runtime::Runtime::new().unwrap();
        let client = tokio_runtime.block_on(async {
            pb::usb_can_client::UsbCanClient::connect(url.to_string())
                .await
                .unwrap()
        });
        Self {
            tokio_runtime: tokio_runtime,
            client: std::cell::RefCell::new(client),
        }
    }
}

impl HandleTrait for GrpcHandle {
    fn read_bulk(&self, data: &mut [u8], _timeout: time::Duration) -> Result<usize, crate::Error> {
        let request = tonic::Request::new(());
        let response = self.tokio_runtime.block_on(async {
            let result = self.client.borrow_mut().read(request).await;
            match result {
                Ok(t) => t.into_inner().recv_buf,
                Err(e) => {
                    eprintln!("{e}");
                    vec![]
                }
            }
        });
        data.copy_from_slice(&response);
        Ok(response.len())
    }
    fn write_bulk(&self, data: &[u8], _timeout: time::Duration) -> Result<usize, crate::Error> {
        let request = tonic::Request::new(WriteRequest {
            send_buf: data.to_vec(),
        });
        self.tokio_runtime.block_on(async {
            self.client.borrow_mut().write(request).await.map_or_else(
                |e| Err(e.into()),
                |t| Ok(t.into_inner().size.try_into().unwrap_or_default()),
            )
        })
    }
}

impl From<tonic::Status> for crate::Error {
    fn from(error: tonic::Status) -> Self {
        crate::Error::GrpcError(error)
    }
}

use crate::{usb::USBHandleTrait, GrpcHandle};
use std::time;

pub mod pb {
    tonic::include_proto!("motor_lib");
}

use pb::UsbCanRequest;

impl GrpcHandle {
    pub fn new(url: &str) -> Self {
        let tokio_context = tokio::runtime::Runtime::new().unwrap();
        let client = tokio_context.block_on(async {
            pb::usb_can_client::UsbCanClient::connect(url.to_string())
                .await
                .unwrap()
        });
        Self {
            tokio_context: tokio_context,
            client: std::cell::RefCell::new(client),
        }
    }
}

impl USBHandleTrait for GrpcHandle {
    fn read_bulk(
        &self,
        data: &mut [u8],
        _timeout: time::Duration,
    ) -> Result<usize, crate::USBError> {
        let request = tonic::Request::new(());
        let response = self.tokio_context.block_on(async {
            let response = self.client.borrow_mut().read(request).await.unwrap();
            return response.into_inner().recv_buf;
        });
        data.copy_from_slice(&response);
        Ok(response.len())
    }
    fn write_bulk(&self, data: &[u8], _timeout: time::Duration) -> Result<usize, crate::USBError> {
        let request = tonic::Request::new(UsbCanRequest {
            send_buf: data.to_vec(),
        });
        self.tokio_context.block_on(async {
            self.client.borrow_mut().write(request).await.unwrap();
        });
        Ok(data.len())
    }
}

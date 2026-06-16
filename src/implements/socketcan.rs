//! Implementation of an abstraction layer for accessing SocketCAN devices.
use crate::HandleTrait;
use socketcan::{
    CanSocket,
    CanFrame, 
    StandardId,
    Socket,
    EmbeddedFrame,
};
use std::time;

/// A handle to read and write a SocketCAN device.
pub struct SocketCANHandle {
    socket: CanSocket,
}

impl SocketCANHandle {
    pub fn new(interface: &str) -> Result<Self, crate::Error> {
        let socket = CanSocket::open(interface)
            .map_err(crate::Error::from)?;
        Ok(Self { socket })
    }
}

impl HandleTrait for SocketCANHandle {
    fn read_bulk(&self, data: &mut [u8], _timeout: time::Duration) -> Result<usize, crate::Error> {
        let frame = self.socket.read_frame()
            .map_err(crate::Error::from)?;
        let frame_data = frame.data();
        data.copy_from_slice(frame_data);
        Ok(frame_data.len())
    }

    fn write_bulk(&self, data: &[u8], _timeout: time::Duration) -> Result<usize, crate::Error> {
        let id = StandardId::new(data[0] as u16).unwrap();
        let frame = CanFrame::new(id, data).unwrap();
        self.socket.write_frame(&frame)
            .map_err(crate::Error::from)?;
        Ok(data.len())
    }
}

impl From<std::io::Error> for crate::Error {
    fn from(error: std::io::Error) -> Self {
        crate::Error::SocketCANError(socketcan::Error::Io(error))
    }
}
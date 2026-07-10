pub struct Joy {
    pub axes: Vec<f32>,
    pub buttons: Vec<i32>,
}

pub mod p9n_interface;
pub mod ps4;
pub mod ps5;
pub mod combination;

pub use p9n_interface::{Axes, Button, Gamepad};
pub use ps4::DualShock4Layout;
pub use ps5::DualSenseLayout;
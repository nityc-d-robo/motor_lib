use super::p9n_interface::GamepadLayout;
use super::Joy;

#[allow(non_snake_case, non_upper_case_globals)]
pub mod AXES_DUALSENSE {
    pub const STICK_LX: usize = 0;
    pub const STICK_LY: usize = 1;
    pub const L2: usize = 2;
    pub const STICK_RX: usize = 3;
    pub const STICK_RY: usize = 4;
    pub const R2: usize = 5;
    pub const DPAD_X: usize = 6;
    pub const DPAD_Y: usize = 7;
}
#[allow(non_snake_case, non_upper_case_globals)]
pub mod BUTTONS_DUALSENSE {
    pub const CROSS: usize = 0;
    pub const CIRCLE: usize = 1;
    pub const TRIANGLE: usize = 2;
    pub const SQUARE: usize = 3;
    pub const L1: usize = 4;
    pub const R1: usize = 5;
    pub const L2: usize = 6;
    pub const R2: usize = 7;
    pub const SELECT: usize = 8;
    pub const START: usize = 9;
    pub const PS: usize = 10;
    pub const STICK_L_PUSH: usize = 11;
    pub const STICK_R_PUSH: usize = 12;
}

pub struct DualSenseLayout;

impl GamepadLayout for DualSenseLayout {
    fn ps(&self, msg: &Joy) -> bool {
        self.is_valid_button(msg, BUTTONS_DUALSENSE::PS)
            && msg.buttons.as_slice()[BUTTONS_DUALSENSE::PS] == 1
    }
    fn l1(&self, msg: &Joy) -> bool {
        self.is_valid_button(msg, BUTTONS_DUALSENSE::L1)
            && msg.buttons.as_slice()[BUTTONS_DUALSENSE::L1] == 1
    }
    fn r1(&self, msg: &Joy) -> bool {
        self.is_valid_button(msg, BUTTONS_DUALSENSE::R1)
            && msg.buttons.as_slice()[BUTTONS_DUALSENSE::R1] == 1
    }
    fn l2(&self, msg: &Joy) -> bool {
        self.is_valid_button(msg, BUTTONS_DUALSENSE::L2)
            && msg.buttons.as_slice()[BUTTONS_DUALSENSE::L2] == 1
    }
    fn r2(&self, msg: &Joy) -> bool {
        self.is_valid_button(msg, BUTTONS_DUALSENSE::R2)
            && msg.buttons.as_slice()[BUTTONS_DUALSENSE::R2] == 1
    }
    fn cross(&self, msg: &Joy) -> bool {
        self.is_valid_button(msg, BUTTONS_DUALSENSE::CROSS)
            && msg.buttons.as_slice()[BUTTONS_DUALSENSE::CROSS] == 1
    }
    fn circle(&self, msg: &Joy) -> bool {
        self.is_valid_button(msg, BUTTONS_DUALSENSE::CIRCLE)
            && msg.buttons.as_slice()[BUTTONS_DUALSENSE::CIRCLE] == 1
    }
    fn triangle(&self, msg: &Joy) -> bool {
        self.is_valid_button(msg, BUTTONS_DUALSENSE::TRIANGLE)
            && msg.buttons.as_slice()[BUTTONS_DUALSENSE::TRIANGLE] == 1
    }
    fn square(&self, msg: &Joy) -> bool {
        self.is_valid_button(msg, BUTTONS_DUALSENSE::SQUARE)
            && msg.buttons.as_slice()[BUTTONS_DUALSENSE::SQUARE] == 1
    }
    fn dpad_left(&self, msg: &Joy) -> bool {
        self.is_valid_axis(msg, AXES_DUALSENSE::DPAD_X)
            && msg.axes.as_slice()[AXES_DUALSENSE::DPAD_X] > 0.0
    }
    fn dpad_right(&self, msg: &Joy) -> bool {
        self.is_valid_axis(msg, AXES_DUALSENSE::DPAD_X)
            && msg.axes.as_slice()[AXES_DUALSENSE::DPAD_X] < 0.0
    }
    fn dpad_up(&self, msg: &Joy) -> bool {
        self.is_valid_axis(msg, AXES_DUALSENSE::DPAD_Y)
            && msg.axes.as_slice()[AXES_DUALSENSE::DPAD_Y] > 0.0
    }
    fn dpad_down(&self, msg: &Joy) -> bool {
        self.is_valid_axis(msg, AXES_DUALSENSE::DPAD_Y)
            && msg.axes.as_slice()[AXES_DUALSENSE::DPAD_Y] < 0.0
    }
    fn select(&self, msg: &Joy) -> bool {
        self.is_valid_button(msg, BUTTONS_DUALSENSE::SELECT)
            && msg.buttons.as_slice()[BUTTONS_DUALSENSE::SELECT] == 1
    }
    fn start(&self, msg: &Joy) -> bool {
        self.is_valid_button(msg, BUTTONS_DUALSENSE::START)
            && msg.buttons.as_slice()[BUTTONS_DUALSENSE::START] == 1
    }

    fn stick_l_x(&self, msg: &Joy) -> f32 {
        if self.is_valid_axis(msg, AXES_DUALSENSE::STICK_LX) {
            msg.axes.as_slice()[AXES_DUALSENSE::STICK_LX]
        } else {
            0.0
        }
    }
    fn stick_l_y(&self, msg: &Joy) -> f32 {
        if self.is_valid_axis(msg, AXES_DUALSENSE::STICK_LY) {
            msg.axes.as_slice()[AXES_DUALSENSE::STICK_LY]
        } else {
            0.0
        }
    }
    fn stick_r_x(&self, msg: &Joy) -> f32 {
        if self.is_valid_axis(msg, AXES_DUALSENSE::STICK_RX) {
            msg.axes.as_slice()[AXES_DUALSENSE::STICK_RX]
        } else {
            0.0
        }
    }
    fn stick_r_y(&self, msg: &Joy) -> f32 {
        if self.is_valid_axis(msg, AXES_DUALSENSE::STICK_RY) {
            msg.axes.as_slice()[AXES_DUALSENSE::STICK_RY]
        } else {
            0.0
        }
    }
}

impl DualSenseLayout {
    pub fn l2_analog(&self, msg: &Joy) -> f32 {
        msg.axes.as_slice()[AXES_DUALSENSE::L2]
    }
    pub fn r2_analog(&self, msg: &Joy) -> f32 {
        msg.axes.as_slice()[AXES_DUALSENSE::R2]
    }

    pub fn stick_left_push(&self, msg: &Joy) -> bool {
        self.is_valid_button(msg, BUTTONS_DUALSENSE::STICK_L_PUSH)
            && msg.buttons.as_slice()[BUTTONS_DUALSENSE::STICK_L_PUSH] == 1
    }

    pub fn stick_right_push(&self, msg: &Joy) -> bool {
        self.is_valid_button(msg, BUTTONS_DUALSENSE::STICK_R_PUSH)
            && msg.buttons.as_slice()[BUTTONS_DUALSENSE::STICK_R_PUSH] == 1
    }
}
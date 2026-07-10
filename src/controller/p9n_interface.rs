use super::*;
// 共通のインターフェース
use strum::IntoEnumIterator;
use strum_macros::EnumIter;

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash, EnumIter)]
pub enum Button {
    PS,
    L1,
    R1,
    L2,
    R2,
    Cross,
    Circle,
    Triangle,
    Square,
    DpadLeft,
    DpadRight,
    DpadUp,
    DpadDown,
    Select,
    Start,
}

pub enum Axes {
    StickLX,
    StickLY,
    StickRX,
    StickRY,
}

pub trait GamepadLayout {
    fn ps(&self, msg: &Joy) -> bool;
    fn l1(&self, msg: &Joy) -> bool;
    fn r1(&self, msg: &Joy) -> bool;
    fn l2(&self, msg: &Joy) -> bool;
    fn r2(&self, msg: &Joy) -> bool;
    fn cross(&self, msg: &Joy) -> bool;
    fn circle(&self, msg: &Joy) -> bool;
    fn triangle(&self, msg: &Joy) -> bool;
    fn square(&self, msg: &Joy) -> bool;
    fn dpad_left(&self, msg: &Joy) -> bool;
    fn dpad_right(&self, msg: &Joy) -> bool;
    fn dpad_up(&self, msg: &Joy) -> bool;
    fn dpad_down(&self, msg: &Joy) -> bool;
    fn select(&self, msg: &Joy) -> bool;
    fn start(&self, msg: &Joy) -> bool;

    fn stick_l_x(&self, msg: &Joy) -> f32;
    fn stick_l_y(&self, msg: &Joy) -> f32;
    fn stick_r_x(&self, msg: &Joy) -> f32;
    fn stick_r_y(&self, msg: &Joy) -> f32;

    // 共通のヘルパー
    fn is_valid_button(&self, msg: &Joy, button: usize) -> bool {
        button < msg.buttons.len()
    }
    fn is_valid_axis(&self, msg: &Joy, axis: usize) -> bool {
        axis < msg.axes.len()
    }
}

pub struct Gamepad<L: GamepadLayout> {
    pub layout: L,
    prev_buttons: Vec<bool>,
}

impl<L: GamepadLayout> Gamepad<L> {
    pub fn new(layout: L) -> Self {
        Self {
            layout,
            prev_buttons: vec![false; Button::iter().count()],
        }
    }
    /// ボタンが現在押されているか（汎用版）
    pub fn pressed(&self, msg: &Joy, button: Button) -> bool {
        match button {
            Button::PS => self.layout.ps(msg),
            Button::L1 => self.layout.l1(msg),
            Button::R1 => self.layout.r1(msg),
            Button::L2 => self.layout.l2(msg),
            Button::R2 => self.layout.r2(msg),
            Button::Cross => self.layout.cross(msg),
            Button::Circle => self.layout.circle(msg),
            Button::Triangle => self.layout.triangle(msg),
            Button::Square => self.layout.square(msg),
            Button::DpadLeft => self.layout.dpad_left(msg),
            Button::DpadRight => self.layout.dpad_right(msg),
            Button::DpadUp => self.layout.dpad_up(msg),
            Button::DpadDown => self.layout.dpad_down(msg),
            Button::Select => self.layout.select(msg),
            Button::Start => self.layout.start(msg),
        }
    }

    /// ボタンの立ち上がり（押された瞬間）を検出
    pub fn pressed_edge(&mut self, msg: &Joy, button: Button) -> bool {
        let idx = button as usize;
        let current = self.pressed(msg, button);
        let previous = self.prev_buttons[idx];
        self.prev_buttons[idx] = current;
        !previous && current
    }

    /// ボタンの立ち下がり（離された瞬間）を検出
    pub fn released_edge(&mut self, msg: &Joy, button: Button) -> bool {
        let idx = button as usize;
        let current = self.pressed(msg, button);
        let previous = self.prev_buttons[idx];
        self.prev_buttons[idx] = current;
        previous && !current
    }

    pub fn axis(&self, msg: &Joy, axis: Axes) -> f32 {
        match axis {
            Axes::StickLX => self.layout.stick_l_x(msg),
            Axes::StickLY => self.layout.stick_l_y(msg),
            Axes::StickRX => self.layout.stick_r_x(msg),
            Axes::StickRY => self.layout.stick_r_y(msg),
        }
    }
}
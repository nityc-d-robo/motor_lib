// src/bin/examples/test_controller.rs
use motor_lib::controller::{Gamepad, Button, DualSenseLayout, Joy};

fn main() {
    let gamepad = Gamepad::new(DualSenseLayout);

    // DualSenseのボタン配列
    // [Cross, Circle, Triangle, Square, L1, R1, L2, R2, Select, Start, PS, StickL, StickR]
    //    0      1        2        3      4    5   6   7     8      9    10    11      12

    // TriangleとL1を押した状態
    let joy = Joy {
        axes: vec![0.0; 8],
        buttons: vec![0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0],
        //                  ↑Triangle  ↑L1
    };

    println!("=== ボタンテスト ===");
    println!("Triangle: {}", gamepad.pressed(&joy, Button::Triangle));
    println!("L1:       {}", gamepad.pressed(&joy, Button::L1));
    println!("Cross:    {}", gamepad.pressed(&joy, Button::Cross));
    println!("Circle:   {}", gamepad.pressed(&joy, Button::Circle));
    println!("Square:   {}", gamepad.pressed(&joy, Button::Square));
    println!("R1:       {}", gamepad.pressed(&joy, Button::R1));

    println!("\n=== スティックテスト ===");
    // 左スティックを右に倒した状態
    let joy_stick = Joy {
        axes: vec![-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
        //           ↑左スティックX=-1.0（右方向）
        buttons: vec![0; 13],
    };

    use motor_lib::controller::Axes;
    println!("左スティックX: {}", gamepad.axis(&joy_stick, Axes::StickLX));
    println!("左スティックY: {}", gamepad.axis(&joy_stick, Axes::StickLY));
    println!("右スティックX: {}", gamepad.axis(&joy_stick, Axes::StickRX));
    println!("右スティックY: {}", gamepad.axis(&joy_stick, Axes::StickRY));
}
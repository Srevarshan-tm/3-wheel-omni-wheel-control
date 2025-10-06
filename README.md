# PS4 Controlled 3-Wheel Omni Robot (Teensy + Bluetooth)

This project allows you to control a **3-wheel omnidirectional robot** using a **PS4 controller** connected via Bluetooth to a **Teensy board**.  
The robot uses **inverse kinematics** to map joystick values from the PS4 controller into motor speeds for smooth omni-directional movement.

---

## 🚀 Features
- Control robot wirelessly with a PS4 controller  
- Supports forward, backward, sideways, diagonal, and rotational movement  
- Deadzone handling for better joystick precision  
- Speed scaling and PWM motor control  
- Modular code structure (matrix-based inverse kinematics + motor driver functions)

---

## ⚙️ Hardware Requirements
- **Teensy 4.x** (or compatible Teensy board with `USBHost_t36` support)  
- **USB Host shield or onboard USB host** (for Bluetooth dongle)  
- **Bluetooth Dongle** (to pair PS4 controller)  
- **3 Omni Wheels** with DC motors  
- **Motor Driver** (e.g., L298N / BTS7960 / similar, depending on motor current)  
- Power supply for motors and Teensy  

---

## 🛠️ Libraries Used
- [`USBHost_t36`](https://github.com/PaulStoffregen/USBHost_t36)  
  - Provides USB Host and PS4 controller support for Teensy

---

## 📐 Inverse Kinematics
The motion of a **3-wheel omni drive** is calculated using the following matrix:
|f1 | | 0.58 -0.33 0.58 | | ax |
| f2 | = | -0.58 -0.33 0.33 | * | ay |
| f3 | | 0.00 0.67 0.33 | | w |


Where:
- `ax` = X-axis movement from joystick  
- `ay` = Y-axis movement from joystick  
- `w`  = rotation input  

The outputs `f1`, `f2`, `f3` are motor speed values for the three wheels.

---

## 🎮 Controls
- **Left joystick (X, Y):** Move the robot in any direction  
- **Right joystick (X):** Rotate robot left/right  
- Deadzone applied to filter out small joystick noise  

---

## 🔧 Pin Configuration
| Motor | PWM Pin | DIR Pin |
|-------|---------|---------|
| M1    | 3       | 4       |
| M2    | 5       | 6       |
| M3    | 9       | 10      |

---

## 🖥️ How to Use
1. Connect Bluetooth dongle to Teensy USB Host port  
2. Pair PS4 controller with the dongle  
3. Upload the code to Teensy  
4. Power up motors and Teensy  
5. Use PS4 joystick to drive the robot  

---

## 📊 Example Serial Output
f1: 120 f2: -90 f3: 45
f1: 0 f2: 0 f3: 0
f1: -255 f2: 200 f3: -130

This shows the PWM values being sent to each motor.

---

## 📝 Future Improvements
- Add **PID speed control** with motor encoders for precise closed-loop movement  
- Use right joystick Y-axis for speed scaling  
- Implement motion smoothing / acceleration limiting  
- Add autonomous modes  

---
##Reference Mterial Used
-https://youtu.be/wwQQnSWqB7A?si=7cMZMWFLcSOIgnyS\n
-https://youtu.be/XG4cODYVbJk?si=Fb91N3qoRo44Q5Th

---





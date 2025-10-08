#include <USBHost_t36.h>
//this library consists of the ubs connection between the ps4 and the teensy 
//also it contains the ps4 library with helps to get the values frm the ps4

USBHost myusb;
JoystickController joystick1(myusb);
BluetoothController bluet(myusb);  

//matrix for the inverse kinematics (modified for your motion logic)
float matrix[3][3] = {
    { 0.0,  1.0,  0.33 },   // f1 → stationary in forward/backward
    { -0.58, -0.33, 0.33 }, // f2
    {  0.58, -0.33, 0.33 }  // f3
};

struct WheelSpeeds {
    float f1, f2, f3;
};

//defining the pins 
const int M1_PWM = 2;
const int M1_DIR = 4;
const int M2_PWM = 3;
const int M2_DIR = 5;
const int M3_PWM = 23;
const int M3_DIR = 21;

//defining the constant 
const int DeadZONE = 20;  
const int MAX_PWM  = 75;   

WheelSpeeds speed_set(float ax, float ay, float w) {
    WheelSpeeds ws;

    // Forward/backward → f1 stays rest
    if (abs(ax) < 0.2 && abs(ay) > abs(ax)) {
        ws.f1 = 0;
        if (ay > 0) { // forward
            ws.f2 = -1;
            ws.f3 = 1;
        } else { // backward
            ws.f2 = 1;
            ws.f3 = -1;
        }
    }
    // Left/right → two same, one opposite
    else if (abs(ay) < 0.2 && abs(ax) > abs(ay)) {
        if (ax > 0) { // right
            ws.f1 = 1;
            ws.f2 = -1;
            ws.f3 = 1;
        } else { // left
            ws.f1 = -1;
            ws.f2 = 1;
            ws.f3 = -1;
        }
    }
    // Rotation → all same direction
    else {
        ws.f1 = w;
        ws.f2 = w;
        ws.f3 = w;
    }

    return ws;
}

//maping the joystick value with the max _joy
float normalizeJoystick(int val) {
    int center = 127;
    int DeadZONE = 20;
    int offset = val - center;

    if (abs(offset) < DeadZONE) return 0;
    return (float)offset / 128.0;
}

//getting the pwm val and also setting the direction of the  motor
int Getting_PWM(float val) {
    if (val > 1) val = 1;
    if (val < -1) val = -1;
    return (int)(val * MAX_PWM);
}

//here i am setting the speed of the motors connected
void setMotor(int pwmPin, int dirPin, int val) {
    if (val >= 0) {
        digitalWrite(dirPin, HIGH);
        analogWrite(pwmPin, val);
    } else {
        digitalWrite(dirPin, LOW);
        analogWrite(pwmPin, -val);
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(M1_PWM, OUTPUT);
    pinMode(M1_DIR, OUTPUT);
    pinMode(M2_PWM, OUTPUT);
    pinMode(M2_DIR, OUTPUT);
    pinMode(M3_PWM, OUTPUT);
    pinMode(M3_DIR, OUTPUT);

    myusb.begin();
}

void loop() {
    myusb.Task();
    int joyX, joyY, joyRot = 0;

    if (joystick1.available()) {
        joyX = joystick1.getAxis(0);   
        joyY = joystick1.getAxis(1); 
        joyRot = joystick1.getAxis(2); 
    }

    // Stop in neutral zone
    if (abs(joyX-127) < 20 && abs(joyY-127) < 20 && abs(joyRot-127) < 20){
        setMotor(M1_PWM, M1_DIR, 0);
        setMotor(M2_PWM, M2_DIR, 0);
        setMotor(M3_PWM, M3_DIR, 0);
        Serial.println("Joystick in neutral zone — Motors stopped");
        return;
    }

    float ax = normalizeJoystick(joyX);
    float ay = -normalizeJoystick(joyY);
    float w  = normalizeJoystick(joyRot);

    WheelSpeeds ws = speed_set(ax, ay, w);

    int pwm1 = Getting_PWM(ws.f1);
    int pwm2 = Getting_PWM(ws.f2);
    int pwm3 = Getting_PWM(ws.f3);

    setMotor(M1_PWM, M1_DIR, pwm1);
    setMotor(M2_PWM, M2_DIR, pwm2);
    setMotor(M3_PWM, M3_DIR, pwm3);

    Serial.print("f1: "); Serial.print(pwm1);
    Serial.print("  f2: "); Serial.print(pwm2);
    Serial.print("  f3: "); Serial.println(pwm3);
}

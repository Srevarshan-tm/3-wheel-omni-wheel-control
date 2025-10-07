#include <USBHost_t36.h>
//this library consists of the ubs connection between the ps4 and the teensy 
//also it contains the ps4 library with helps to get the values frm the ps4

USBHost myusb;
JoystickController joystick1(myusb);
BluetoothController bluet(myusb);  

//matrix for the inverse kinematics 
float matrix[3][3] = {
    { 0.58, -0.33, 0.58 },
    { -0.58, -0.33, 0.33 },
    { 0.0,   0.67,  0.33 }\
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
const int MAX_JOY  = 127;   
const int MAX_PWM  = 75;   


WheelSpeeds speed_set(float ax, float ay, float w) {
    //calculating the speed using the matrix that is defined  at the start
    WheelSpeeds ws;
    ws.f1 = ax * matrix[0][0] + ay * matrix[0][1] + w * matrix[0][2];
    ws.f2 = ax * matrix[1][0] + ay * matrix[1][1] + w * matrix[1][2];
    ws.f3 = ax * matrix[2][0] + ay * matrix[2][1] + w * matrix[2][2];
    return ws;
}
//maping the joystick value with the max _joy
float mapJoystick(int val) {
    if (abs(val) < DeadZONE) return 0;     
    return (float)val / MAX_JOY;           
}
//getting the pwm val and also setting the direction of the  motor
int Getting_PWM(float val) {
    if (val > 1) val = 1;
    if (val < -1) val = -1;
    return (int)(val * MAX_PWM);
}
//here i am settind the speed of the motors connected
void setMotor(int pwmPin, int dirPin, int val) {
    if (val >= 0) {
        digitalWrite(dirPin, HIGH);
        analogWrite(pwmPin, val-);
    } else {
        digitalWrite(dirPin, LOW);
        analogWrite(pwmPin, -val);
    }
}


void setup() {
    Serial.begin(9600);
    //setting the pinmode for each pin defined eariler
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

    if (joystick1.available()) {
        //getting the valu from the joy stick  
        int joyX =joystick1.getAxis(0);   
        int joyY =joystick1.getAxis(1); 
        int joyRot =joystick1.getAxis(2); 
    }
    // If joystick is near center → stop all motors
    if (abs(joyX) < 20 && abs(joyY) < 20 && abs(joyRot) < 20){
        setMotor(M1_PWM, M1_DIR, 0);
        setMotor(M2_PWM, M2_DIR, 0);
        setMotor(M3_PWM, M3_DIR, 0);
        Serial.println("Joystick in neutral zone — Motors stopped");
        return;
    }
        //sending the raw value from the joystick to the mapping function  
        float ax = mapJoystick(joyX);
        float ay = mapJoystick(-joyY); 
        float w  = mapJoystick(joyRot);
        WheelSpeeds ws = speed_set(ax, ay, w);
        int pwm1 = Getting_PWM(ws.f1);
        int pwm2 = Getting_PWM(ws.f2);
        int pwm3 = Getting_PWM(ws.f3);
        setMotor(M1_PWM, M1_DIR, pwm1);
        setMotor(M2_PWM, M2_DIR, pwm2);
        setMotor(M3_PWM, M3_DIR, pwm3);
        //for testing so printing the values in the serial monitor 
        Serial.print("f1: "); Serial.print(pwm1);
        Serial.print("  f2: "); Serial.print(pwm2);
        Serial.print("  f3: "); Serial.println(pwm3);
        Serial.print("  :joyx "); Serial.println(joyX);
        Serial.print("  :joyy "); Serial.println(joyY);
        Serial.print("  :joyrot"); Serial.println(joyRot);
        delay(2000);

    }
}

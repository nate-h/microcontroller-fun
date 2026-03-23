/*
  Joystick Servo and Motor Demo

  Parts:
  - 1 analog joystick module
  - 1 small servo motor
  - 1 DC motor
  - 1 H-bridge motor driver

  Wiring:
  - ---
  - Joystick VCC -> 5V
  - Joystick GND -> GND
  - Joystick VRX -> A0
  - Joystick VRY -> A1
  - ---
  - Servo signal (white wire) -> pin 9
  - Servo power (red wire) -> 5V
  - Servo ground (black wire) -> GND
  - ---
  - L293D pin 1 (EN1,2) -> Arduino pin 5 for PWM speed control
  - L293D pin 2 (1A) -> Arduino pin 7
  - L293D pin 7 (2A) -> Arduino pin 8
  - L293D pin 3 (1Y) -> one motor terminal
  - L293D pin 6 (2Y) -> the other motor terminal
  - L293D pin 8 (Vcc2) -> battery or external motor supply
  - L293D pin 16 (Vcc1) -> 5V
  - L293D pins 4, 5, 12, and 13 -> GND
  - Arduino GND and motor supply GND must be connected together

  Notes:
  - Left/right moves the servo.
  - Up/down controls motor speed and direction through the H-bridge.
  - This sketch does not use the joystick pushbutton, so the 10k resistor is not needed here.
  - Powering the motor from the Arduino 5V pin is not recommended.
*/
#include <Servo.h>

const int pinVRX = A0;
const int pinVRY = A1;

const int servoPin = 9;
const int motorEnablePin = 5;
const int motorIn1Pin = 7;
const int motorIn2Pin = 8;

const int centerX = 512;
const int centerY = 512;
const int deadzone = 60;

Servo steeringServo;

int applyDeadzone(int value, int center, int zone) {
  int delta = value - center;

  if (abs(delta) < zone) {
    return 0;
  }

  return delta;
}

void setMotor(int signedSpeed) {
  int pwm = constrain(abs(signedSpeed), 0, 255);

  if (signedSpeed > 0) {
    digitalWrite(motorIn1Pin, HIGH);
    digitalWrite(motorIn2Pin, LOW);
  } else if (signedSpeed < 0) {
    digitalWrite(motorIn1Pin, LOW);
    digitalWrite(motorIn2Pin, HIGH);
  } else {
    digitalWrite(motorIn1Pin, LOW);
    digitalWrite(motorIn2Pin, LOW);
  }

  analogWrite(motorEnablePin, pwm);
}

void setup() {
  Serial.begin(9600);

  pinMode(motorEnablePin, OUTPUT);
  pinMode(motorIn1Pin, OUTPUT);
  pinMode(motorIn2Pin, OUTPUT);

  steeringServo.attach(servoPin);
  steeringServo.write(90);

  setMotor(0);
}

void loop() {
  int x = analogRead(pinVRX);
  int y = analogRead(pinVRY);

  int xDelta = applyDeadzone(x, centerX, deadzone);
  int yDelta = applyDeadzone(y, centerY, deadzone);

  int servoAngle = 90;

  if (xDelta != 0) {
    servoAngle = map(x, 0, 1023, 0, 180);
    servoAngle = constrain(servoAngle, 0, 180);
  }

  steeringServo.write(servoAngle);

  // Most joystick modules read lower values when pushed up, so invert Y
  // so pushing up means forward motor motion.
  int motorSpeed = map(y, 0, 1023, 255, -255);

  if (yDelta == 0) {
    motorSpeed = 0;
  }

  setMotor(motorSpeed);

  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" | Servo: ");
  Serial.print(servoAngle);
  Serial.print(" || Y: ");
  Serial.print(y);
  Serial.print(" | Motor: ");
  Serial.println(motorSpeed);

  delay(20);
}

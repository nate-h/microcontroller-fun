/*
  Joystick Servo Demo Setup

  Parts:
  - 1 analog joystick module
  - 1 servo motor
  - 4 LEDs
  - 4 resistors for the LEDs (220 ohm typical)

  Wiring:
  - Joystick VCC -> 5V
  - Joystick GND -> GND
  - Joystick VRX -> A0
  - Joystick VRY -> A1
  - Servo signal -> pin 9
  - Servo power -> 5V
  - Servo ground -> GND
  - Left LED anode -> pin 5 through a resistor
  - Right LED anode -> pin 6 through a resistor
  - Up LED anode -> pin 3 through a resistor
  - Down LED anode -> pin 11 through a resistor
  - All LED cathodes -> GND

  Notes:
  - Open the Serial Monitor at 9600 baud to see joystick values and servo angle.
  - Small servos may work from the board 5V pin; larger servos should use external power with shared GND.
*/
#include <Servo.h>

const int pinVRX = A0;
const int pinVRY = A1;

// PWM LED pins (safe with servo)
const int ledLeft = 5;
const int ledRight = 6;
const int ledUp = 3;
const int ledDown = 11;

// Servo
const int servoPin = 9;
Servo myServo;

int centerX = 512;
int centerY = 512;
const int deadzone = 50;

void setup() {
  Serial.begin(9600);

  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);
  pinMode(ledUp, OUTPUT);
  pinMode(ledDown, OUTPUT);

  myServo.attach(servoPin);
}

void loop() {
  int x = analogRead(pinVRX);
  int y = analogRead(pinVRY);

  int dx = x - centerX;
  int dy = centerY - y; // flip Y

  if (abs(dx) < deadzone) dx = 0;
  if (abs(dy) < deadzone) dy = 0;

  int leftVal  = dx < 0 ? map(-dx, 0, 512, 0, 255) : 0;
  int rightVal = dx > 0 ? map(dx, 0, 512, 0, 255) : 0;
  int upVal    = dy > 0 ? map(dy, 0, 512, 0, 255) : 0;
  int downVal  = dy < 0 ? map(-dy, 0, 512, 0, 255) : 0;

  leftVal  = constrain(leftVal, 0, 255);
  rightVal = constrain(rightVal, 0, 255);
  upVal    = constrain(upVal, 0, 255);
  downVal  = constrain(downVal, 0, 255);

  analogWrite(ledLeft, leftVal);
  analogWrite(ledRight, rightVal);
  analogWrite(ledUp, upVal);
  analogWrite(ledDown, downVal);

  // Servo control (X axis)
  int angle = map(x, 0, 1023, 0, 180);
  angle = constrain(angle, 0, 180);
  myServo.write(angle);

  Serial.print("X: "); Serial.print(x);
  Serial.print(" | Servo: "); Serial.print(angle);
  Serial.print(" || Y: "); Serial.print(y);
  Serial.print(" || L: "); Serial.print(leftVal);
  Serial.print(" R: "); Serial.print(rightVal);
  Serial.print(" U: "); Serial.print(upVal);
  Serial.print(" D: "); Serial.print(downVal);
  Serial.println();

  delay(20);
}

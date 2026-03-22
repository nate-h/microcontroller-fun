/*
  Joystick Simple Demo Setup

  Parts:
  - 1 analog joystick module
  - 4 LEDs
  - 4 resistors for the LEDs (220 ohm typical)

  Wiring:
  - Joystick VCC -> 5V
  - Joystick GND -> GND
  - Joystick VRX -> A0
  - Joystick VRY -> A1
  - Up LED anode -> pin 9 through a resistor
  - Down LED anode -> pin 10 through a resistor
  - Left LED anode -> pin 5 through a resistor
  - Right LED anode -> pin 6 through a resistor
  - All LED cathodes -> GND

  Notes:
  - Open the Serial Monitor at 9600 baud to see joystick values and LED levels.
*/
const int pinVRX = A0;
const int pinVRY = A1;

const int ledUp = 9;
const int ledDown = 10;
const int ledLeft = 5;
const int ledRight = 6;

int centerX = 512;
int centerY = 512;
const int deadzone = 50;

void setup() {
  pinMode(ledUp, OUTPUT);
  pinMode(ledDown, OUTPUT);
  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  int x = analogRead(pinVRX);
  int y = analogRead(pinVRY);

  int dx = x - centerX;
  int dy = centerY - y;  // flipped Y axis

  if (abs(dx) < deadzone) dx = 0;
  if (abs(dy) < deadzone) dy = 0;

  int rightVal = dx > 0 ? map(dx, 0, 512, 0, 255) : 0;
  int leftVal  = dx < 0 ? map(-dx, 0, 512, 0, 255) : 0;

  int upVal    = dy > 0 ? map(dy, 0, 512, 0, 255) : 0;
  int downVal  = dy < 0 ? map(-dy, 0, 512, 0, 255) : 0;

  rightVal = constrain(rightVal, 0, 255);
  leftVal  = constrain(leftVal, 0, 255);
  upVal    = constrain(upVal, 0, 255);
  downVal  = constrain(downVal, 0, 255);

  analogWrite(ledRight, rightVal);
  analogWrite(ledLeft, leftVal);
  analogWrite(ledUp, upVal);
  analogWrite(ledDown, downVal);

  Serial.print("Raw X: "); Serial.print(x);
  Serial.print(" | Raw Y: "); Serial.print(y);
  Serial.print(" || dx: "); Serial.print(dx);
  Serial.print(" | dy: "); Serial.print(dy);
  Serial.print(" || L: "); Serial.print(leftVal);
  Serial.print(" R: "); Serial.print(rightVal);
  Serial.print(" U: "); Serial.print(upVal);
  Serial.print(" D: "); Serial.print(downVal);
  Serial.println();

  delay(50);
}

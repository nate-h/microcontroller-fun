/*
  Variable LED Demo Setup

  Parts:
  - 1 potentiometer
  - 1 LED
  - 1 resistor for the LED (220 ohm typical)

  Wiring:
  - Potentiometer left pin -> 5V
  - Potentiometer middle pin -> A0
  - Potentiometer right pin -> GND
  - LED anode -> pin 9 through the resistor
  - LED cathode -> GND
*/
const int potPin = A0;   // Potentiometer input
const int ledPin = 9;    // PWM output pin (must have ~)

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int potValue = analogRead(potPin);      // 0–1023
  int brightness = map(potValue, 0, 1023, 0, 255); // scale to PWM range

  analogWrite(ledPin, brightness);        // set LED brightness

  delay(5); // small delay for stability
}

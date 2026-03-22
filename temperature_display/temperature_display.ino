/*
  Temperature Display Demo Setup

  Parts:
  - 1 16x2 LCD
  - 1 analog temperature sensor compatible with this formula (for example TMP36)
  - 1 pushbutton
  - 1 10k ohm resistor for the button pulldown

  Wiring:
  - LCD RS -> pin 12
  - LCD E -> pin 11
  - LCD D4 -> pin 5
  - LCD D5 -> pin 4
  - LCD D6 -> pin 3
  - LCD D7 -> pin 2
  - LCD VSS -> GND
  - LCD VDD -> 5V
  - LCD RW -> GND
  - LCD VO -> middle pin of a contrast potentiometer
  - Contrast potentiometer outer pins -> 5V and GND
  - Temperature sensor output -> A0
  - Temperature sensor power -> 5V
  - Temperature sensor ground -> GND
  - Pushbutton one side -> 5V
  - Pushbutton other side -> pin 7
  - 10k ohm resistor from pin 7 -> GND

  Notes:
  - The sketch expects the temperature formula used by a TMP36-style sensor.
  - Open the Serial Monitor at 9600 baud if you want to inspect readings while testing.
*/
#include <LiquidCrystal.h>

// LCD pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int tempPin = A0;
const int buttonPin = 7;

int unitMode = 0; // 0=C,1=F,2=K,3=R
bool lastButton = false;

void setup() {
  lcd.begin(16, 2);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // --- Button handling (edge detect) ---
  bool currentButton = digitalRead(buttonPin);
  if (currentButton && !lastButton) {
    unitMode = (unitMode + 1) % 4;
    delay(200); // debounce
  }
  lastButton = currentButton;

  // --- Read temperature ---
  int raw = analogRead(tempPin);
  float voltage = raw * (5.0 / 1023.0);
  float tempC = (voltage - 0.5) * 100.0;

  float displayTemp;
  String unit;

  switch (unitMode) {
    case 0:
      displayTemp = tempC;
      unit = "C";
      break;
    case 1:
      displayTemp = tempC * 9.0/5.0 + 32.0;
      unit = "F";
      break;
    case 2:
      displayTemp = tempC + 273.15;
      unit = "K";
      break;
    case 3:
      displayTemp = tempC * 9.0/5.0 + 491.67;
      unit = "R";
      break;
  }

  // --- Display ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");

  lcd.setCursor(0, 1);
  lcd.print(displayTemp, 1);
  lcd.print(" ");
  lcd.print(unit);

  delay(500);
}

/*
  Traffic Light Pedestrian Demo Setup

  Parts:
  - 5 LEDs
  - 5 resistors for the LEDs (220 ohm typical)
  - 1 pushbutton
  - 1 passive 3-pin buzzer module

  Wiring:
  - Car red LED anode -> pin 8 through a resistor
  - Car yellow LED anode -> pin 9 through a resistor
  - Car green LED anode -> pin 10 through a resistor
  - Pedestrian red LED anode -> pin 11 through a resistor
  - Pedestrian green LED anode -> pin 12 through a resistor
  - All LED cathodes -> GND
  - Pushbutton one side -> pin 2
  - Pushbutton other side -> GND
  - Buzzer module GND -> GND
  - Buzzer module VCC -> 5V
  - Buzzer module I/O -> pin 6

  Notes:
  - The button uses INPUT_PULLUP, so it reads pressed when connected to GND.
  - This sketch assumes the buzzer module is passive, so the Arduino generates the tone.
  - Press the button while cars have green to request a walk cycle.
*/
const int carRedPin = 8;
const int carYellowPin = 9;
const int carGreenPin = 10;

const int pedRedPin = 11;
const int pedGreenPin = 12;

const int buttonPin = 2;
const int buzzerPin = 6;

enum LightState {
  CARS_GO,
  CARS_SLOW,
  ALL_STOP_1,
  PEDESTRIANS_GO,
  PEDESTRIANS_FLASH,
  ALL_STOP_2
};

LightState state = CARS_GO;

unsigned long stateStartedAt = 0;
bool pedestrianRequested = false;
bool lastButtonPressed = false;

const unsigned long yellowTimeMs = 2000;
const unsigned long allStopTimeMs = 1000;
const unsigned long walkTimeMs = 5000;
const unsigned long flashIntervalMs = 300;

void playBuzzer(unsigned int frequency) {
  tone(buzzerPin, frequency);
}

void playBuzzer(unsigned int frequency, unsigned long durationMs) {
  tone(buzzerPin, frequency, durationMs);
}

void stopBuzzer() {
  noTone(buzzerPin);
}

bool buttonPressed() {
  return digitalRead(buttonPin) == LOW;
}

bool buttonJustPressed() {
  bool currentPressed = buttonPressed();
  bool justPressed = currentPressed && !lastButtonPressed;
  lastButtonPressed = currentPressed;
  return justPressed;
}

void setLights(bool carRed, bool carYellow, bool carGreen, bool pedRed, bool pedGreen) {
  digitalWrite(carRedPin, carRed);
  digitalWrite(carYellowPin, carYellow);
  digitalWrite(carGreenPin, carGreen);
  digitalWrite(pedRedPin, pedRed);
  digitalWrite(pedGreenPin, pedGreen);
}

void setState(LightState newState) {
  state = newState;
  stateStartedAt = millis();
}

void enterCarsGo() {
  setLights(false, false, true, true, false);
  stopBuzzer();
  pedestrianRequested = false;
  setState(CARS_GO);
}

void enterCarsSlow() {
  setLights(false, true, false, true, false);
  stopBuzzer();
  setState(CARS_SLOW);
}

void enterAllStop1() {
  setLights(true, false, false, true, false);
  stopBuzzer();
  setState(ALL_STOP_1);
}

void enterPedestriansGo() {
  setLights(true, false, false, false, true);
  playBuzzer(880);
  setState(PEDESTRIANS_GO);
}

void enterPedestriansFlash() {
  setLights(true, false, false, false, true);
  stopBuzzer();
  setState(PEDESTRIANS_FLASH);
}

void enterAllStop2() {
  setLights(true, false, false, true, false);
  stopBuzzer();
  setState(ALL_STOP_2);
}

void setup() {
  pinMode(carRedPin, OUTPUT);
  pinMode(carYellowPin, OUTPUT);
  pinMode(carGreenPin, OUTPUT);
  pinMode(pedRedPin, OUTPUT);
  pinMode(pedGreenPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  enterCarsGo();
}

void loop() {
  if (buttonJustPressed() && state == CARS_GO) {
    pedestrianRequested = true;
  }

  unsigned long elapsed = millis() - stateStartedAt;

  switch (state) {
    case CARS_GO:
      if (pedestrianRequested) {
        enterCarsSlow();
      }
      break;

    case CARS_SLOW:
      if (elapsed >= yellowTimeMs) {
        enterAllStop1();
      }
      break;

    case ALL_STOP_1:
      if (elapsed >= allStopTimeMs) {
        enterPedestriansGo();
      }
      break;

    case PEDESTRIANS_GO:
      if ((elapsed / 250) % 2 == 0) {
        playBuzzer(880);
      } else {
        stopBuzzer();
      }

      if (elapsed >= walkTimeMs) {
        enterPedestriansFlash();
      }
      break;

    case PEDESTRIANS_FLASH:
      if ((elapsed / flashIntervalMs) % 2 == 0) {
        digitalWrite(pedGreenPin, HIGH);
        playBuzzer(660, 80);
      } else {
        digitalWrite(pedGreenPin, LOW);
        stopBuzzer();
      }

      if (elapsed >= 3000) {
        enterAllStop2();
      }
      break;

    case ALL_STOP_2:
      if (elapsed >= allStopTimeMs) {
        enterCarsGo();
      }
      break;
  }
}

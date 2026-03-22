/*
  Piano Demo Setup

  Parts:
  - 8 pushbuttons
  - 1 piezo buzzer or small speaker

  Wiring:
  - Buzzer positive -> pin 11
  - Buzzer negative -> GND
  - Button 1 one side -> pin 2
  - Button 2 one side -> pin 3
  - Button 3 one side -> pin 4
  - Button 4 one side -> pin 5
  - Button 5 one side -> pin 6
  - Button 6 one side -> pin 7
  - Button 7 one side -> pin 8
  - Button 8 one side -> pin 9
  - Other side of every button -> GND

  Notes:
  - The buttons use INPUT_PULLUP, so a button press connects its pin to GND.
  - Multiple buttons can be pressed together for a simple chord effect.
*/
// 8-key Arduino piano with pseudo-chords by fast note cycling

const int speakerPin = 11;

const int keyPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
const int notes[8]   = {262, 294, 330, 349, 392, 440, 494, 523}; // C4 D4 E4 F4 G4 A4 B4 C5

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(keyPins[i], INPUT_PULLUP);
  }
}

void loop() {
  int pressed[8];
  int pressedCount = 0;

  // Collect all pressed keys
  for (int i = 0; i < 8; i++) {
    if (digitalRead(keyPins[i]) == LOW) {
      pressed[pressedCount] = i;
      pressedCount++;
    }
  }

  if (pressedCount == 0) {
    noTone(speakerPin);
    return;
  }

  if (pressedCount == 1) {
    tone(speakerPin, notes[pressed[0]]);
    delay(5);
    return;
  }

  // Multiple keys pressed: rapidly alternate between them
  for (int i = 0; i < pressedCount; i++) {
    tone(speakerPin, notes[pressed[i]]);
    delay(3);  // smaller = more blended, too small may sound weak
  }
}

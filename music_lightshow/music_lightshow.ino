/*
  Music Lightshow Demo Setup

  Parts:
  - 1 piezo buzzer or small speaker
  - 7 LEDs
  - 7 resistors for the LEDs (220 ohm typical)

  Wiring:
  - Buzzer positive -> pin 11
  - Buzzer negative -> GND
  - LED 1 anode -> pin 2 through a resistor
  - LED 2 anode -> pin 3 through a resistor
  - LED 3 anode -> pin 4 through a resistor
  - LED 4 anode -> pin 5 through a resistor
  - LED 5 anode -> pin 6 through a resistor
  - LED 6 anode -> pin 7 through a resistor
  - LED 7 anode -> pin 8 through a resistor
  - All LED cathodes -> GND
*/
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494

#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784

const int speakerPin = 11;
const int ledPins[7] = {2,3,4,5,6,7,8};

// Melody (Ode to Joy)
const int melody[] = {
NOTE_E4, NOTE_E4, NOTE_F4, NOTE_G4,
NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4,
NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4,
NOTE_E4, NOTE_D4, NOTE_D4,

NOTE_E4, NOTE_E4, NOTE_F4, NOTE_G4,
NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4,
NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4,
NOTE_D4, NOTE_C4, NOTE_C4
};

const int durations[] = {
300,300,300,300,
300,300,300,300,
300,300,300,300,
450,150,600,

300,300,300,300,
300,300,300,300,
300,300,300,300,
450,150,700
};

void allOff() {
  for(int i=0;i<7;i++)
    digitalWrite(ledPins[i],LOW);
}

int noteToLed(int freq) {

  switch(freq) {

    case NOTE_C4:
    case NOTE_C5: return 0;

    case NOTE_D4:
    case NOTE_D5: return 1;

    case NOTE_E4:
    case NOTE_E5: return 2;

    case NOTE_F4:
    case NOTE_F5: return 3;

    case NOTE_G4:
    case NOTE_G5: return 4;

    case NOTE_A4: return 5;
    case NOTE_B4: return 6;
  }

  return -1;
}

void playSong() {

  int length = sizeof(melody)/sizeof(melody[0]);

  for(int i=0;i<length;i++) {

    int note = melody[i];
    int duration = durations[i];

    int led = noteToLed(note);

    if(led >= 0)
      digitalWrite(ledPins[led],HIGH);

    tone(speakerPin, note, duration);
    delay(duration);

    if(led >= 0)
      digitalWrite(ledPins[led],LOW);

    delay(40);
  }

  noTone(speakerPin);
}

void setup() {

  pinMode(speakerPin,OUTPUT);

  for(int i=0;i<7;i++)
    pinMode(ledPins[i],OUTPUT);
}

void loop() {

  playSong();

  delay(2000);
}

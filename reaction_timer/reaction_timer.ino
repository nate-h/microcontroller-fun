/*
  Reaction Timer Demo Setup

  Parts:
  - 1 pushbutton
  - 1 LED
  - 1 piezo buzzer
  - 1 resistor for the LED (220 ohm typical)

  Wiring:
  - LED anode -> pin 9 through the resistor
  - LED cathode -> GND
  - Pushbutton one side -> pin 2
  - Pushbutton other side -> GND
  - Buzzer positive -> pin 10
  - Buzzer negative -> GND

  Notes:
  - The button uses INPUT_PULLUP, so it reads pressed when connected to GND.
  - Open the Serial Monitor at 9600 baud to see the reaction time in milliseconds.
*/
const int ledPin = 9;
const int buttonPin = 2;
const int buzzerPin = 10;

enum GameState {
  WAITING_TO_START,
  WAITING_RANDOM_DELAY,
  WAITING_FOR_REACTION,
  SHOW_RESULT,
  TOO_EARLY
};

GameState state = WAITING_TO_START;

unsigned long stateStartedAt = 0;
unsigned long randomDelayMs = 0;
unsigned long reactionStartedAt = 0;
unsigned long reactionTimeMs = 0;

bool lastButtonPressed = false;

bool buttonPressed() {
  return digitalRead(buttonPin) == LOW;
}

bool buttonJustPressed() {
  bool currentPressed = buttonPressed();
  bool justPressed = currentPressed && !lastButtonPressed;
  lastButtonPressed = currentPressed;
  return justPressed;
}

void setState(GameState newState) {
  state = newState;
  stateStartedAt = millis();
}

void startRound() {
  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);

  randomDelayMs = random(1500, 5001);
  reactionTimeMs = 0;
  setState(WAITING_RANDOM_DELAY);

  Serial.println("Get ready...");
}

void showReactionSignal() {
  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, 1000);
  reactionStartedAt = millis();
  setState(WAITING_FOR_REACTION);

  Serial.println("GO!");
}

void showTooEarly() {
  digitalWrite(ledPin, LOW);
  tone(buzzerPin, 220, 500);
  setState(TOO_EARLY);

  Serial.println("Too early! Wait for the signal, then press again to restart.");
}

void showResult() {
  digitalWrite(ledPin, LOW);
  tone(buzzerPin, 1400, 150);
  setState(SHOW_RESULT);

  Serial.print("Reaction time: ");
  Serial.print(reactionTimeMs);
  Serial.println(" ms");
  Serial.println("Press the button to play again.");
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);
  randomSeed(analogRead(A0));

  Serial.println("Reaction Timer");
  Serial.println("Press the button to start.");
}

void loop() {
  bool justPressed = buttonJustPressed();

  switch (state) {
    case WAITING_TO_START:
      if (justPressed) {
        startRound();
      }
      break;

    case WAITING_RANDOM_DELAY:
      if (justPressed) {
        showTooEarly();
      } else if (millis() - stateStartedAt >= randomDelayMs) {
        showReactionSignal();
      }
      break;

    case WAITING_FOR_REACTION:
      if (justPressed) {
        noTone(buzzerPin);
        reactionTimeMs = millis() - reactionStartedAt;
        showResult();
      }
      break;

    case SHOW_RESULT:
      if (justPressed) {
        startRound();
      }
      break;

    case TOO_EARLY:
      if (justPressed) {
        startRound();
      }
      break;
  }
}

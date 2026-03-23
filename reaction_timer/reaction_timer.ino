/*
  Reaction Timer Demo Setup

  Parts:
  - 1 SSD1306 128x64 I2C OLED display
  - 1 pushbutton
  - 1 LED
  - 1 piezo buzzer
  - 1 resistor for the LED (220 ohm typical)

  Wiring:
  - OLED VCC -> 5V
  - OLED GND -> GND
  - OLED SDA -> board SDA
  - OLED SCL -> board SCL
  - LED anode -> pin 9 through the resistor
  - LED cathode -> GND
  - Pushbutton one side -> pin 2
  - Pushbutton other side -> GND
  - Buzzer positive -> pin 10
  - Buzzer negative -> GND

  Notes:
  - The button uses INPUT_PULLUP, so it reads pressed when connected to GND.
  - Install these libraries before uploading:
    - Adafruit GFX Library
    - Adafruit SSD1306
  - In Arduino IDE: Sketch -> Include Library -> Manage Libraries, then search for those names.
  - This sketch expects the display at I2C address 0x3C.
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

const int ledPin = 9;
const int buttonPin = 2;
const int buzzerPin = 10;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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

void drawCenteredText(const char* text, int y, int textSize) {
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextSize(textSize);
  display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  int x = (SCREEN_WIDTH - w) / 2;
  display.setCursor(x < 0 ? 0 : x, y);
  display.print(text);
}

void drawScreen() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  switch (state) {
    case WAITING_TO_START:
      drawCenteredText("REACTION", 10, 2);
      drawCenteredText("TIMER", 30, 2);
      drawCenteredText("Press button", 54, 1);
      break;

    case WAITING_RANDOM_DELAY:
      drawCenteredText("Get ready...", 14, 2);
      drawCenteredText("Wait for GO", 44, 1);
      break;

    case WAITING_FOR_REACTION:
      drawCenteredText("GO!", 18, 3);
      drawCenteredText("Press now", 54, 1);
      break;

    case SHOW_RESULT:
      drawCenteredText("Time", 8, 2);
      display.setTextSize(2);
      display.setCursor(20, 30);
      display.print(reactionTimeMs);
      display.print(" ms");
      drawCenteredText("Press to retry", 54, 1);
      break;

    case TOO_EARLY:
      drawCenteredText("Too early!", 16, 2);
      drawCenteredText("Press to retry", 54, 1);
      break;
  }

  display.display();
}

void startRound() {
  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);

  randomDelayMs = random(1500, 5001);
  reactionTimeMs = 0;
  setState(WAITING_RANDOM_DELAY);
}

void showReactionSignal() {
  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, 1000);
  reactionStartedAt = millis();
  setState(WAITING_FOR_REACTION);
}

void showTooEarly() {
  digitalWrite(ledPin, LOW);
  tone(buzzerPin, 220, 500);
  setState(TOO_EARLY);
}

void showResult() {
  digitalWrite(ledPin, LOW);
  tone(buzzerPin, 1400, 150);
  setState(SHOW_RESULT);
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  randomSeed(analogRead(A0));

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (true) {}
  }

  drawScreen();
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

  drawScreen();
}

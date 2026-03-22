/*
  Snake Demo Setup

  Parts:
  - 1 SSD1306 128x64 I2C OLED display
  - 4 pushbuttons

  Wiring:
  - OLED VCC -> 5V
  - OLED GND -> GND
  - OLED SDA -> board SDA
  - OLED SCL -> board SCL
  - Up button one side -> pin 2
  - Down button one side -> pin 3
  - Left button one side -> pin 4
  - Right button one side -> pin 5
  - Other side of every button -> GND

  Notes:
  - The buttons use INPUT_PULLUP, so a button press connects its pin to GND.
  - This sketch expects the display at I2C address 0x3C.
  - Install these libraries before uploading:
    - Adafruit GFX Library
    - Adafruit SSD1306
  - In Arduino IDE: Sketch -> Include Library -> Manage Libraries, then search for those names.
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

#define BTN_UP 2
#define BTN_DOWN 3
#define BTN_LEFT 4
#define BTN_RIGHT 5

#define CELL 4
#define TOP_BAR 10

#define GRID_W 31
#define GRID_H 13
#define MAX_LEN 100

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

struct Pt {
  byte x;
  byte y;
};

Pt snake[MAX_LEN];
int snakeLen;

int dx, dy;
int nextDx, nextDy;

Pt food;
int score;
bool started;
bool gameOver;
unsigned long lastStep;
const unsigned long stepDelay = 150;

bool pressed(int pin) {
  return digitalRead(pin) == LOW;
}

bool onSnake(byte x, byte y) {
  for (int i = 0; i < snakeLen; i++) {
    if (snake[i].x == x && snake[i].y == y) return true;
  }
  return false;
}

void placeFood() {
  do {
    food.x = random(GRID_W);
    food.y = random(GRID_H);
  } while (onSnake(food.x, food.y));
}

void resetGame() {
  snakeLen = 3;
  snake[0] = {10, 6};
  snake[1] = {9, 6};
  snake[2] = {8, 6};

  dx = 1; dy = 0;
  nextDx = 1; nextDy = 0;

  score = 0;
  started = false;
  gameOver = false;
  lastStep = millis();

  placeFood();
}

void readButtons() {
  if (pressed(BTN_UP) && dy != 1) {
    nextDx = 0; nextDy = -1; started = true;
  } else if (pressed(BTN_DOWN) && dy != -1) {
    nextDx = 0; nextDy = 1; started = true;
  } else if (pressed(BTN_LEFT) && dx != 1) {
    nextDx = -1; nextDy = 0; started = true;
  } else if (pressed(BTN_RIGHT) && dx != -1) {
    nextDx = 1; nextDy = 0; started = true;
  }
}

void stepGame() {
  if (!started || gameOver) return;
  if (millis() - lastStep < stepDelay) return;
  lastStep = millis();

  dx = nextDx;
  dy = nextDy;

  int nx = snake[0].x + dx;
  int ny = snake[0].y + dy;

  if (nx < 0 || nx >= GRID_W || ny < 0 || ny >= GRID_H) {
    gameOver = true;
    return;
  }

  for (int i = 0; i < snakeLen; i++) {
    if (snake[i].x == nx && snake[i].y == ny) {
      gameOver = true;
      return;
    }
  }

  for (int i = snakeLen; i > 0; i--) {
    snake[i] = snake[i - 1];
  }
  snake[0].x = nx;
  snake[0].y = ny;

  if (nx == food.x && ny == food.y) {
    if (snakeLen < MAX_LEN - 1) snakeLen++;
    score++;
    placeFood();
  } else {
    // tail effectively removed by not increasing snakeLen
  }
}

void drawCell(byte gx, byte gy) {
  int px = 2 + gx * CELL;
  int py = TOP_BAR + 2 + gy * CELL;
  display.fillRect(px, py, CELL, CELL, SSD1306_WHITE);
}

void drawGame() {
  display.clearDisplay();

  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 2);
  display.print("Score:");
  display.print(score);

  drawCell(food.x, food.y);

  for (int i = 0; i < snakeLen; i++) {
    drawCell(snake[i].x, snake[i].y);
  }

  if (!started && !gameOver) {
    display.setCursor(18, 28);
    display.print("Press a button");
  }

  if (gameOver) {
    display.setCursor(28, 28);
    display.print("GAME OVER");
  }

  display.display();
}

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  randomSeed(analogRead(A0));

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (true) {}
  }

  resetGame();
  drawGame();
}

void loop() {
  readButtons();

  if (gameOver) {
    if (pressed(BTN_UP) || pressed(BTN_DOWN) || pressed(BTN_LEFT) || pressed(BTN_RIGHT)) {
      delay(200);
      resetGame();
    }
  } else {
    stepGame();
  }

  drawGame();
}

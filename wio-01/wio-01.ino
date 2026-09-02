#include "TFT_eSPI.h"

TFT_eSPI tft;

#define TFT_WIDTH 320
#define TFT_HEIGHT 240

// Tasti 5 direzioni Wio Terminal
#define JOY_UP     WIO_5S_UP
#define JOY_DOWN   WIO_5S_DOWN
#define JOY_LEFT   WIO_5S_LEFT
#define JOY_RIGHT  WIO_5S_RIGHT
#define JOY_PRESS  WIO_5S_PRESS

// Layout tastiera 4x4
const char* keys[4][4] = {
  {"1", "2", "3", "A"},
  {"4", "5", "6", "B"},
  {"7", "8", "9", "C"},
  {"*", "0", "#", "D"}
};

const char* correctPassword = "A1B2C3D4";
char enteredPassword[9] = ""; // 8 + null terminator
int passIndex = 0;

int cursorX = 0;
int cursorY = 0;

// Dimensioni celle
int cellW = TFT_WIDTH / 4;
int cellH = (TFT_HEIGHT - 20) / 4;

void setup() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  // Joystick digitale
  pinMode(JOY_UP, INPUT_PULLUP);
  pinMode(JOY_DOWN, INPUT_PULLUP);
  pinMode(JOY_LEFT, INPUT_PULLUP);
  pinMode(JOY_RIGHT, INPUT_PULLUP);
  pinMode(JOY_PRESS, INPUT_PULLUP);

  drawKeypad();
  highlightKey(cursorX, cursorY);
}

void loop() {
  static unsigned long lastMove = 0;
  static bool lastPress = false;

  if (millis() - lastMove > 150) {
    if (digitalRead(JOY_LEFT) == LOW) {
      moveCursor(-1, 0);
      lastMove = millis();
    } else if (digitalRead(JOY_RIGHT) == LOW) {
      moveCursor(1, 0);
      lastMove = millis();
    } else if (digitalRead(JOY_UP) == LOW) {
      moveCursor(0, -1);
      lastMove = millis();
    } else if (digitalRead(JOY_DOWN) == LOW) {
      moveCursor(0, 1);
      lastMove = millis();
    }
  }

  bool pressNow = digitalRead(JOY_PRESS) == LOW;
  if (pressNow && !lastPress) {
    handleKeyPress();
  }
  lastPress = pressNow;
}

void moveCursor(int dx, int dy) {
  unhighlightKey(cursorX, cursorY);
  cursorX = constrain(cursorX + dx, 0, 3);
  cursorY = constrain(cursorY + dy, 0, 3);
  highlightKey(cursorX, cursorY);
}

void drawKeypad() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      int xpos = x * cellW;
      int ypos = y * cellH;
      tft.drawRect(xpos, ypos, cellW, cellH, TFT_WHITE);
      tft.drawString(keys[y][x], xpos + cellW/2, ypos + cellH/2);
    }
  }

  updatePasswordBar();
}

void highlightKey(int x, int y) {
  int xpos = x * cellW;
  int ypos = y * cellH;
  tft.fillRect(xpos+2, ypos+2, cellW-4, cellH-4, TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(keys[y][x], xpos + cellW/2, ypos + cellH/2);
}

void unhighlightKey(int x, int y) {
  int xpos = x * cellW;
  int ypos = y * cellH;
  tft.fillRect(xpos+2, ypos+2, cellW-4, cellH-4, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawRect(xpos, ypos, cellW, cellH, TFT_WHITE);
  tft.drawString(keys[y][x], xpos + cellW/2, ypos + cellH/2);
}

void handleKeyPress() {
  if (passIndex >= 8) return;

  const char* key = keys[cursorY][cursorX];
  enteredPassword[passIndex++] = key[0];
  enteredPassword[passIndex] = '\0';

  updatePasswordBar();

  if (passIndex == 8) {
    delay(300);
    checkPassword();
  }
}

void updatePasswordBar() {
  tft.fillRect(0, TFT_HEIGHT - 20, TFT_WIDTH, 20, TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
  tft.drawString(enteredPassword, 10, TFT_HEIGHT - 20);
}

void checkPassword() {
  if (strcmp(enteredPassword, correctPassword) == 0) {
    tft.fillScreen(TFT_GREEN);
    tft.setTextColor(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(3);
    tft.drawString("ACCESSO OK", TFT_WIDTH/2, TFT_HEIGHT/2);
  } else {
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(3);
    tft.drawString("PASSWORD ERRATA", TFT_WIDTH/2, TFT_HEIGHT/2);
  }

  delay(3000);
  passIndex = 0;
  enteredPassword[0] = '\0';
  drawKeypad();
}

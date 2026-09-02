#include <Adafruit_NeoPixel.h>
#include "TFT_eSPI.h"

// === LED STRIP SETUP ===
#define LED_PIN    BCM11
#define LED_COUNT  200
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// === WIO TERMINAL SETUP ===
TFT_eSPI tft;

#define TFT_WIDTH  320
#define TFT_HEIGHT 240

// Joystick direzionale Wio Terminal
#define JOY_UP     WIO_5S_UP
#define JOY_DOWN   WIO_5S_DOWN
#define JOY_LEFT   WIO_5S_LEFT
#define JOY_RIGHT  WIO_5S_RIGHT
#define JOY_PRESS  WIO_5S_PRESS

// Tastiera virtuale 4x4

const char* keys[4][4] = {
  {"1", "2", "3", "a"},
  {"4", "5", "6", "y"},
  {"7", "8", "9", "c"},
  {"*", "0", "#", "p"}
};

const char* correctPassword = "3c395yap";
char enteredPassword[9] = ""; // 8 + '\0'
int passIndex = 0;

// Cursore attuale
int cursorX = 0;
int cursorY = 0;

// Celle grafiche
int cellW = TFT_WIDTH / 4;
int cellH = (TFT_HEIGHT - 20) / 4;

// LED blinking
bool blinkingError = true;
unsigned long lastBlink = 0;
bool ledState = false;

void setup() {
  // LCD
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  // Joystick digitale
  pinMode(JOY_UP, INPUT_PULLUP);
  pinMode(JOY_DOWN, INPUT_PULLUP);
  pinMode(JOY_LEFT, INPUT_PULLUP);
  pinMode(JOY_RIGHT, INPUT_PULLUP);
  pinMode(JOY_PRESS, INPUT_PULLUP);

  // LED WS2812
  strip.begin();
  strip.show();            // spegne tutti i LED
  strip.setBrightness(200); // moderato

  drawKeypad();
  highlightKey(cursorX, cursorY);
}

void loop() {
  // === INPUT JOYSTICK ===
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

  // === BLINK ROSSO LED ===
  if (blinkingError && millis() - lastBlink > 500) {
    ledState = !ledState;
    uint32_t color = ledState ? strip.Color(255, 0, 0) : strip.Color(0, 0, 0);
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }
    strip.show();
    lastBlink = millis();
  }
}

// === KEYPAD UI ===

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

void moveCursor(int dx, int dy) {
  unhighlightKey(cursorX, cursorY);
  cursorX = constrain(cursorX + dx, 0, 3);
  cursorY = constrain(cursorY + dy, 0, 3);
  highlightKey(cursorX, cursorY);
}

void updatePasswordBar() {
  tft.fillRect(0, TFT_HEIGHT - 20, TFT_WIDTH, 20, TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
  tft.drawString(enteredPassword, 10, TFT_HEIGHT - 20);
}

// === LOGICA ===

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

void checkPassword() {
  blinkingError = false; // stop lampeggio

  if (strcmp(enteredPassword, correctPassword) == 0) {
    // Schermata verde OK
    tft.fillScreen(TFT_GREEN);
    tft.setTextColor(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(3);
    tft.drawString("ACCESSO OK", TFT_WIDTH/2, TFT_HEIGHT/2);

    // LED VERDI
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
    }
    strip.show();
    delay(1000);

  } else {
    // Schermata errore
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(3);
    tft.drawString("PASSWORD ERRATA", TFT_WIDTH/2, TFT_HEIGHT/2);

    // Riattiva lampeggio rosso
    blinkingError = true;
  }

  delay(3000);
  passIndex = 0;
  enteredPassword[0] = '\0';
  drawKeypad();
}

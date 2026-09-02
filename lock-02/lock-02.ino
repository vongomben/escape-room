#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>

// === TFT ===
#define TFT_CS   10
#define TFT_RST  9
#define TFT_DC   8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// === BUTTON ===
#define BUTTON_PIN 2 // collegato a GND
// Il bottone usa INPUT_PULLUP → LOW = premuto

// === NEOPIXEL ===
#define LED_PIN     3
#define LED_COUNT   200

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// === VARIABILI LED ===
bool blinkingError = true;
bool ledState = false;
unsigned long lastBlink = 0;

void setup(void) {
  Serial.begin(9600);

  // TFT
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);

  // Bottone
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // NeoPixel
  strip.begin();
  strip.setBrightness(50);
  strip.show(); // spegni tutti
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  // === Display ===
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 30);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);

  if (buttonState == LOW) {
    // Bottone premuto
    tft.print("Password: A1B2C3D4");

    blinkingError = false;

    // LED VERDI fissi per 1s
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
    }
    strip.show();
    delay(1000);
  } else {
    // Bottone non premuto
    tft.print("Inserisci la chiave");

    blinkingError = true;
  }

  // === LED Lampeggio Rosso (non bloccante) ===
  if (blinkingError && millis() - lastBlink > 500) {
    ledState = !ledState;
    uint32_t color = ledState ? strip.Color(255, 0, 0) : strip.Color(0, 0, 0);
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }
    strip.show();
    lastBlink = millis();
  }

  delay(300); // debounce display
}

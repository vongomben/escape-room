// ESCAPE ROOM v 0.1
//
//
// per cambiare la password
// editare il testo tra le virgolette.
//
// per problemi scrivere a d.gomba@officine.cc

String password = "AD993DDD";

#include <TFT_eSPI.h>
#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN BCM10      // D2 (BCM10)
#define NEOPIXEL_PIN BCM22    // D1 (BCM27)
#define NUM_PIXELS 60

TFT_eSPI tft = TFT_eSPI();
Adafruit_NeoPixel strip(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool lastButtonState = HIGH;
bool ledState = false;
unsigned long lastBlinkTime = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Setup schermo
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(30, 80);
  tft.print("Trova la Chiave");

  tft.setCursor(30, 110);
  tft.print("Disarma la Teca");


  // Setup NeoPixel
  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Spegni tutto
}

void loop() {
  bool currentButtonState = digitalRead(BUTTON_PIN);

  // === Chiave inserita: HIGH → LOW ===
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    // Messaggio sullo schermo
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 30);
    tft.print("la password e'");
    tft.setCursor(10, 60);
    tft.print(password);

    // LED VERDI per 1 secondo
    for (int i = 0; i < NUM_PIXELS; i++) {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
    }
    strip.show();
    delay(1000);

    strip.clear();
    strip.show();
  }

  // === Chiave rimossa: LOW → HIGH ===
  if (lastButtonState == LOW && currentButtonState == HIGH) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 30);
    tft.print("inserisci la chiave");
  }

  // === LED lampeggio rosso ===
  if (currentButtonState == HIGH && millis() - lastBlinkTime > 500) {
    ledState = !ledState;
    for (int i = 0; i < NUM_PIXELS; i++) {
      strip.setPixelColor(i, ledState ? strip.Color(255, 0, 0) : 0);
    }
    strip.show();
    lastBlinkTime = millis();
  }

  lastButtonState = currentButtonState;
}

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS   10
#define TFT_RST  9
#define TFT_DC   8
#define BUTTON_PIN 2   // Bottone collegato a GND

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup(void) {
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP); // ✅ Pull-up interno abilitato
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 30);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);

  if (buttonState == LOW) { // ✅ Premuto (LOW per via del pull-up)
    tft.print("Password: A1B2C3D4");
  } else {
    tft.print("Inserisci la chiave");
  }

  delay(300); // debounce semplice
}

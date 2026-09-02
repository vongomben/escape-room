#include <TFT_eSPI.h>  // Libreria grafica per Wio Terminal

#define BUTTON_PIN BCM10    // BCM10 sulla Wio Terminal → corrisponde a D2

TFT_eSPI tft = TFT_eSPI();

bool lastState = HIGH;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Bottone con pull-up interno

  tft.begin();
  tft.setRotation(3);                 // Orientamento orizzontale
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);

  tft.setCursor(10, 30);
  tft.print("inserisci la chiave");
}

void loop() {
  bool currentState = digitalRead(BUTTON_PIN);

  // Rileva transizione da HIGH → LOW (pressione bottone)
  if (lastState == HIGH && currentState == LOW) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 30);
    tft.print("la password e'");
    tft.setCursor(10, 60);
    tft.print("ciaociao");
  }

  // Rileva rilascio del bottone (LOW → HIGH), torna a messaggio iniziale
  if (lastState == LOW && currentState == HIGH) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 30);
    tft.print("inserisci la chiave");
  }

  lastState = currentState;
}

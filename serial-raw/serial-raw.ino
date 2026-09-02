#include <SoftwareSerial.h>

#define RX_PIN 6
#define TX_PIN 5

SoftwareSerial meshSerial(RX_PIN, TX_PIN);

void setup() {
  Serial.begin(9600);
  meshSerial.begin(9600);
  Serial.println("🧪 Lettura grezza da Meshtastic:");
}

void loop() {
  while (meshSerial.available()) {
    char c = meshSerial.read();
    Serial.print(c);
  }
}

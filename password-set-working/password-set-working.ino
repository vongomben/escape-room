#include <SoftwareSerial.h>

#define RX_PIN 6
#define TX_PIN 5
#define BAUD_RATE 9600

SoftwareSerial meshSerial(RX_PIN, TX_PIN);
String incomingMessage = "";
String password = "";*-

void setup() {
  Serial.begin(9600);
  meshSerial.begin(BAUD_RATE);
  Serial.println("🔐 Meshbot pronto. Aspetto messaggi...");
}

void loop() {
  // Legge caratteri in arrivo e li accumula in una stringa
  while (meshSerial.available()) {
    char c = meshSerial.read();

    // Se riceviamo un carattere di fine riga (come newline), processiamo il messaggio
    if (c == '\n' || c == '\r') {
      incomingMessage.trim();
      if (incomingMessage.length() > 0) {
        Serial.print("📩 Ricevuto: ");
        Serial.println(incomingMessage);
        
        if (incomingMessage.indexOf("/password1") != -1) {
          int sep = incomingMessage.indexOf(" ", incomingMessage.indexOf("/password1"));
          if (sep != -1 && sep + 1 < incomingMessage.length()) {
            password = incomingMessage.substring(sep + 1);
            Serial.print("✅ Password salvata: ");
            Serial.println(password);

            // Risposta via rete Meshtastic
            String risposta = "/text la password del lock è stata modificata con " + password;
            meshSerial.println(risposta);
          } else {
            Serial.println("❌ Errore: nessuna password trovata.");
          }
        } else {
          Serial.println("❔ Comando sconosciuto.");
        }
      }
      incomingMessage = "";  // reset messaggio
    } else {
      incomingMessage += c;
    }
  }
}

#include <ETH.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <Wire.h>
#include <Adafruit_PN532.h>
#include <HTTPClient.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define RSTO_PIN 5
#define IRQ_PIN 18

Adafruit_PN532 nfc(IRQ_PIN, RSTO_PIN);

WiFiMulti wifiMulti;

String farben[] = {"Rot", "Gelb", "Blau", "Gruen", "Schwarz"};

void setup(void) {
  Serial.begin(115200);
  Serial.println("Programm started !");
  Wire.begin();
  nfc.begin();

  uint32_t versionData = nfc.getFirmwareVersion();
  if (!versionData) {
    Serial.print("Didn't find PN53x board");
    while (1);
  }

  wifiMulti.addAP("SSID", "Passwort");

  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Verbindung wird hergestellt...");
  }

  Serial.println("Verbindung erfolgreich !");
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    Serial.println("Reading Tag Content:");
    for (uint8_t block = 7; block < 8; block++) {
      uint8_t data[16];
      success = nfc.mifareclassic_ReadDataBlock(block, data);
      String firstByteAsString = String(data[0], HEX);

      if (success && WiFi.status() == WL_CONNECTED) {
        sendHttpRequest(getCardName(firstByteAsString));
      } else {
        Serial.println("Read failed or WiFi not connected!");
      }

      Serial.println("");
    }
    delay(3000);  // Kurze Verzögerung, um die Prozessorlast zu reduzieren
  }
}

String getCardName(String hexMod) {

String hex = hexMod;
String farbe;
String nummer;

if(hex.length() == 1){
  
  farbe = "0";
  nummer = hex.substring(0, 1);

}
else{
  
   farbe = hex.substring(0, 1);
   nummer = hex.substring(1, 2);

}

  if(nummer == "a"){
    nummer = "Aussetzen";
  }
  if(nummer == "b"){
    nummer = "Reverse";
  }
  if(nummer == "c"){
    nummer = "2Ziehen";
  }
  if(nummer == "d"){
    nummer = "Farbauswahl";
  }
  if(nummer == "e"){
    nummer = "4Ziehen";
  }

String antwort = farben[farbe.toInt()] + String(nummer);

  return antwort ;
}

void sendHttpRequest(String karte) {
  HTTPClient http;

  Serial.println("[HTTP] begin...");

  String serverUrl = "http://IpAddress:3000/ergebnis/" + karte;

  http.begin(serverUrl);

  Serial.println("[HTTP] GET...");

  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}


 
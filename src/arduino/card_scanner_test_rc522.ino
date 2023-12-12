#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         22           
#define SS_PIN          21           

String lastCardName = "";

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// Define an array of UUIDs
const char* uuids[] = {
  "1391d43",
  "3263784d",
  "b1fcbd6",
  "a24aee4b",
  "b1a37b9",
  "a21d9c4d",
  "b1e8316",
  "929b444d",
  "f12a5c9",
  "717f5347",
  "c1b6af9"
};

// Corresponding names for each UUID
const char* names[] = {
  "Card1",
  "Card2",
  "Card3",
  "Card4",
  "Card5",
  "Card6",
  "Card7",
  "Card8",
  "Card9",
  "Card10",
  "Card11"
};

unsigned long lastReadTime = 0;
unsigned long readInterval = 1000; // Interval between card reading operations in milliseconds

WiFiMulti wifiMulti;

void setup() {
  Serial.begin(115200);
  SPI.begin();      // Initiate SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522

  Serial.println();
  Serial.println("Waiting for connection to Wi-Fi...");

  wifiMulti.addAP("SSID", "Password");  // Add your WLAN SSID and password

  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }

  Serial.println("Connected to Wi-Fi!");
}

void loop() {
  // Check if it's time to read a card
  if (millis() - lastReadTime > readInterval) {
    if (readCard()) {
      sendHttpRequest(lastCardName);
    }
    lastReadTime = millis();
  }
}

bool readCard() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  // Print UID
  Serial.print("Card read. UID: ");
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; ++i) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Search for the UID in the array and get the corresponding name
  const char* cardName = getCardName(uidString.c_str());

  lastCardName = cardName;
  Serial.print("Card name: ");
  Serial.println(cardName);

  return true;
}

const char* getCardName(const char* uid) {
  for (int i = 0; i < sizeof(uuids) / sizeof(uuids[0]); i++) {
    if (strcmp(uid, uuids[i]) == 0) {
      return names[i];
    }
  }
  return "Unknown Card";
}

void sendHttpRequest(String card) {
  HTTPClient http;

  Serial.println("[HTTP] begin...");

  // Create the URL with the name as a query parameter
  String serverUrl = "http://IP_Address:3000/result/";
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; ++i) {
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  const char* cardName = getCardName(uidString.c_str());

  serverUrl += cardName;

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

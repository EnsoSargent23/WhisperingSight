// imports
#include <SPI.h>
#include <MFRC522.h>


// Pin Definition
#define SS_PIN 10
#define RST_PIN 9

// Instatiate Motor and RFID Module

MFRC522 rfid(SS_PIN, RST_PIN); 

byte nuidPICC[4];

// Setup for first time
void setup() { 
  // Serial Begin
  Serial.begin(9600);
  // Init SPI bus
  SPI.begin(); 
  // Init MFRC522
  rfid.PCD_Init();  
}

// iteration
void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!rfid.PICC_IsNewCardPresent()){
    return;
  }
  // Verify if the NUID has been read
  if (!rfid.PICC_ReadCardSerial()){
    return;
  }

    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

  String id = "ID: "+String(nuidPICC[0])+" : "+String(nuidPICC[1])+" : "+String(nuidPICC[2])+" : "+String(nuidPICC[3]);
   Serial.println(id);
  


  delay(1000);

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
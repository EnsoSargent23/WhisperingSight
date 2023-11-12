// imports
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h> 

// Pin Definition
#define SS_PIN 10
#define RST_PIN 9

// Instatiate Motor and RFID Module
Servo motor;
MFRC522 rfid(SS_PIN, RST_PIN); 



// Setup for first time
void setup() { 
  // Serial Begin
  Serial.begin(9600);
  // Servo Attach
  motor.attach(6);
  // Pin Setting
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  // Init SPI bus
  SPI.begin(); 
  // Init MFRC522
  rfid.PCD_Init();  
}

// iteration
void loop() {
  // Reset the loop if no new card present on the reader
  if (!rfid.PICC_IsNewCardPresent()){
    return;
  }
  // Verify if the NUID has been read
  if (!rfid.PICC_ReadCardSerial()){
    return;
  }
  
  // Send UID of token to the serialport
  Serial.println(rfid.uid)


  delay(1000);

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

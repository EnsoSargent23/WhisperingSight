// imports 
#include <SPI.h>
#include <MFRC522.h>


// Pin Setting
#define RST_PIN1 9
#define SS_PIN1 10

#define RST_PIN2 5
#define SS_PIN2 6

#define RST_PIN3 2
#define SS_PIN3 3

// RFID-Module Instances
MFRC522 mfrc522_1(SS_PIN1, RST_PIN1);
MFRC522 mfrc522_2(SS_PIN2, RST_PIN2);
MFRC522 mfrc522_3(SS_PIN3, RST_PIN3);

// Setup
void setup() {
    // Serial Begin
  Serial.begin(9600);
  // SPI start
  SPI.begin();

  // Initialisation of Modules
  mfrc522_1.PCD_Init();
  mfrc522_2.PCD_Init();
  mfrc522_3.PCD_Init();
}

void loop() {
  // reading of triple signal
}

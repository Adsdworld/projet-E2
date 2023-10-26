#include <SPI.h> // SPI
#include <MFRC522.h> //RFID

#define SS_PIN D10
#define RST_PIN D9

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  String carte_id = GetId();
  if (carte_id != 0000 && carte_id != "") {
  Serial.println("carte_id send to database: " + carte_id);
  }
}

String GetId() {
  String id = "";

  while (!rfid.PICC_IsNewCardPresent()) {
    //delay(50);
    //Serial.print("on attends");
    //return id;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      id += String(rfid.uid.uidByte[i], HEX);
    }

    Serial.print("Un badge est détecté. L'UID du tag est: ");
    Serial.println(id);

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    return id;
  }

  return id;
}

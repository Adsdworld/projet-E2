#include <SPI.h> // SPI
#include <MFRC522.h> //RFID
#define SS_PIN D10 //Use D before for the WIFI CARD
#define RST_PIN D9 //Use D before for the WIFI CARD

MFRC522 rfid(SS_PIN, RST_PIN);
String carte_id;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  carte_id = GetId();
  Serial.println(carte_id);
}

String GetId() {
  while (!rfid.PICC_IsNewCardPresent()) {//il attends dans cette boucle, ajout de millis plus tard
  }
  if (!rfid.PICC_ReadCardSerial()) {
    String id;
    for (byte i = 0; i < 4; i++) {
      id += String(rfid.uid.uidByte[i], HEX);
    }
    rfid.PICC_HaltA();     //permet d'arrêter la communication avec la carte
    rfid.PCD_StopCrypto1();//permet d'arrêter la communication avec la carte
    if(id==0000 || id=="0000"){
      return GetId();
    }else{
      return id;
    }
  }
  return GetId();
}

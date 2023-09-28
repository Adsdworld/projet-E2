#include <SPI.h> // SPI
#include <MFRC522.h> //RFID

#define SS_PIN 10
#define RST_PIN 9

// Déclaration
MFRC522 rfid(SS_PIN, RST_PIN);

byte nuidPICC[4];


void setup() {

  Serial.begin(9600);

  // Init SPI bus
  SPI.begin();

  // Init MFRC522
  rfid.PCD_Init();

}

void loop() {
  // Initialisé la boucle si aucun badge n'est présent
  if (!rfid.PICC_IsNewCardPresent())
    return;
  
  // Vérifier la présence d'un nouveau badge
  if ( !rfid.PICC_ReadCardSerial())
    return;
  
  // Enregistrer l'ID du badge (4 octets)
  for (byte i = 0; i < 4; i++)
  {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
  // Affichage de l'ID
  Serial.println("Un badge est détecté");
  Serial.println(" L'UID du tag est:");
  for (byte i = 0; i < 4; i++)
  {
    Serial.println(nuidPICC[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Re-init RFID
  rfid.PICC_HaltA(); // Arrêt PICC
  rfid.PCD_StopCrypto1(); // Stop encryption on PCD



}

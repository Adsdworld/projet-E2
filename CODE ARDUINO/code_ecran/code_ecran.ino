/*
qinad onclique sur un bouton si il n'y a pas de changment d'écran inverser couleurs text et rectfill
vérif int > ou < solde

écran get information
écran sending information
écran information send

possibilité de modifier son code




prénom / vérifier présence enveloppe


*/

/****************************************************************************************************************************************
* Main:                                                                                                                                 *
*   Contient les Variables globales.                                                                                                    *
****************************************************************************************************************************************/
#include <SPI.h>
String carte;
//String stringCarteCode;
    //String stringCarteCode = String(""); // Créez une chaîne vide

int intCarteCode;
int solde;
String carte_id;
String id;
String pad;
bool MainSetup(int position, int totalPosition){
  Serial.println(String(position)+"/"+String(totalPosition)+" Main ...");
  SPI.begin();
  Serial.println("Main prêt !");
  return true;
}

/****************************************************************************************************************************************
* UART COMMUNICATION:                                                                                                                   *
*   Contient les fonctions de communications                                                                                            *
****************************************************************************************************************************************/
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#define txPin 8 
#define rxPin 7 
bool COMSetup(int position, int totalPosition) {
  Serial.begin(115200);
  Serial.println(String(position)+"/"+String(totalPosition)+" Communication ...");
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial3.begin(9600);
  Serial.println("Communication prête !");
  return true;
}
int CommunicationDelay=1000;
//int ConsoleRefreshDelay=1000;
int TimeoutMsgResponse=1000;
int TimeoutArduinoUno=20000;
int SlaveTimeout=60000;
unsigned long Timeout;

void sendKeepAlive() {
  Serial3.print("KA");
  Serial.print("Keep alive");
}
void sendMsgToSlave(String message) {
  Serial3.print(message);
  Serial.println("Un message à été envoyé:"+message);
  delay(CommunicationDelay*3);
}
void sendMsgToSlaveWithConfirmation(String message) {
  Serial.println("\n***Sending '"+message+"' with confirmation to Slave");
  String receivedMessage = "";
  while (true){
    Serial3.print(message);
    delay(CommunicationDelay*3);
    //delay(10);
    if (Serial3.available()){
      receivedMessage = "";
      delay(CommunicationDelay); // Wait for the short message to arrive
      Serial.print("New data available from Slave");
      while (Serial3.available() > 0) {
        char serialData = Serial3.read();
        receivedMessage += String(serialData);
      }
      if (receivedMessage=="OK"){
        Serial.println("\n***Successfuly sended '"+message+"' with confirmation to Slave");
        return;
      } else {
        Serial.println("\nMessage reçu:"+receivedMessage);
      }
    }
    Serial.print("."+String(Serial3.available()));
  }
}
String ReadInstantMsgFromArduino(){
  String StringBuilder="";
  while(true){
    if (0<Serial.available()){
      delay(TimeoutMsgResponse); //laisser un peu de temps pour que le court message arrive
      while(Serial.available()>0){
        char Serialdata =Serial.read();
        StringBuilder+=String(Serialdata);
      }
      return StringBuilder;
    }
  }
  return "NO MESSAGE FOUND";
}
int ExtractFieldValue(String dataReceived, String fieldName) {
  const size_t capacity = JSON_OBJECT_SIZE(4) + 60; //ici il y a 4 valeurs: id, carte_id, solde, carte _code >>> à ajuster en fonction des données reçus
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, dataReceived);

  if (error) {
    Serial.print("Erreur de parsing JSON: ");
    Serial.println(error.c_str());
    return -1; // Valeur par défaut si le parsing échoue
  }

  // Vérifiez si le champ spécifié existe dans le JSON
  if (doc.containsKey(fieldName)) {
    return doc[fieldName].as<int>(); // Renvoie la valeur du champ sous forme d'entier
  } else {
    Serial.println(fieldName+" non trouvé dans le JSON");
    return -1; // Champ non trouvé, renvoie une valeur par défaut
  }
}



/****************************************************************************************************************************************
* SCREEN:                                                                                                                   *
*   Contient les fonctions pour l'écran                                                                                                   *
****************************************************************************************************************************************/
#include <XPT2046_Touchscreen.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#define CS_PIN_TOUCH_SCREEN  47 //SS + CS = Slave Select
#define TFT_DC  49
#define TFT_CS 53
#define RST_PIN 48
  XPT2046_Touchscreen ts(CS_PIN_TOUCH_SCREEN);
  Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, RST_PIN);
bool ScreenSetup(int position, int totalPosition) {
  Serial.println(String(position)+"/"+String(totalPosition)+" Ecran ...");
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  pinMode(CS_PIN_TOUCH_SCREEN, OUTPUT);
  digitalWrite(CS_PIN_TOUCH_SCREEN, HIGH);
  tft.begin();
  ts.begin();
  Serial.println("Ecran prêt !");
  return true;
}

/****************************************************************************************************************************************
* RFID:                                                                                                                   *
*   Contient les fonctions pour le RFID                                                                                                 *
****************************************************************************************************************************************/
// RST identique pour RST ECRAN & RFID
#include <MFRC522.h> //RFID
#define CS_PIN_RFID 43 
#define RST_PIN_RFID 41 
MFRC522 rfid(CS_PIN_RFID, RST_PIN_RFID);
bool RfidSetup(int position, int totalPosition){
  Serial.println(String(position)+"/"+String(totalPosition)+" Rfid ...");
  rfid.PCD_Init();
  pinMode(CS_PIN_RFID, OUTPUT);
  digitalWrite(CS_PIN_RFID, HIGH);
  Serial.println("Rfid prêt !");
  return true;
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

/****************************************************************************************************************************************
* DIGICODE:                                                                                                                   *
*   Contient les fonctions pour le digicode                                                                                             *
****************************************************************************************************************************************/
const int Ligne = 4;
const int Colonne = 4;
String hexaBouton[Ligne][Colonne] = {
  {"1", "2", "3", "A"},
  {"4", "5", "6", "B"},
  {"7", "8", "9", "C"},
  {"*", "0", "#", "D"}
};
int lignesPins[Ligne] = {22, 24, 26, 28}; // Broches pour les lignes (D0 à D3)
int colonnesPins[Colonne] = {30, 32, 34, 36};
bool DigicodeSetup(int position, int totalPosition){
  Serial.println(String(position)+"/"+String(totalPosition)+" Digicode ...");
  for (int i = 0; i < Colonne; i++) {
    pinMode(colonnesPins[i], OUTPUT);
    digitalWrite(colonnesPins[i], HIGH);
  }
  for (int i = 0; i < Ligne; i++) {
    pinMode(lignesPins[i], INPUT_PULLUP);
  }
  Serial.println("Digicode prêt !");
  return true;
}
void getNumber(){
  String boutonPresse; // stocke le caractère
  bool play = true; // controle la boucle principale
  while (play){ // boucle principale
    for (int i = 0; i < Colonne; i++) {
        digitalWrite(colonnesPins[i], HIGH); 
    }
    for (int i = 0; i < Colonne; i++) {
      // Activer la colonne en cours
      digitalWrite(colonnesPins[i], LOW); 
      // Lire les lignes
      for (int j = 0; j < Ligne; j++) {
        if (digitalRead(lignesPins[j]) == LOW) {
          // Attendez que le bouton soit relâché
          while (digitalRead(lignesPins[j]) == LOW) {
            //delay(10); //10ms
          }
          // Un bouton a été pressé dans la ligne j et la colonne i
          boutonPresse = hexaBouton[j][i];
          Serial.println(boutonPresse);
          pad += hexaBouton[j][i];
          return;
          play = false;
          break;
        }
      }
      // Désactiver la colonne en cours
      digitalWrite(colonnesPins[i], HIGH);
    }
  }
}






void setup() {
  COMSetup(1, 5);
  MainSetup(2, 5);
  ScreenSetup(3, 5);
  RfidSetup(4, 5);
  DigicodeSetup(5, 5);

  tft.setRotation(1);
  ts.setRotation(2);
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextSize(2);

  Serial.println("La carte Méga est prête !");
}

void loop() {
  /*boolean istouched = ts.touched();
  TS_Point p = ts.getPoint();
  tft.fillRect(50, 180, 140, 60, ILI9341_WHITE);
  tft.setTextColor(ILI9341_DARKGREEN);
  tft.setCursor(10, 180);tft.print("X = ");tft.print(p.x);
  tft.setCursor(10, 210);tft.print("Y = ");tft.print(p.y);
  istouched=false;
  delay(100);*/
  Welcome();
}
void menu(){
  bool testouch=false;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(50, 10);tft.print("Votre solde est de ");
  tft.setCursor(140, 40);tft.print(solde);tft.print("$");
  tft.setCursor(10, 80);tft.print("Que voulez-vous faire?");
  tft.setTextColor(ILI9341_WHITE);
  tft.fillRoundRect(5,135,94,24,10,ILI9341_BLACK);tft.setCursor(10, 140);tft.print("Retrait");
  tft.fillRoundRect(115,135,74,24,10,ILI9341_BLACK);tft.setCursor(120, 140);tft.print("Depot");
  tft.fillRoundRect(205,135,104,24,10,ILI9341_BLACK);tft.setCursor(210, 140);tft.print("Virement");
  tft.fillRoundRect(5,195,74,24,10,ILI9341_BLACK);tft.setCursor(10, 200);tft.print("Finir");
  while(testouch==false){
    boolean touch = ts.touched();
    TS_Point p = ts.getPoint();
    if(touch && p.x>2300 && p.x<2600 && p.y>450 &&p.y<1500){
    retrait();
    testouch=true;
    delay(100);
    }
    if(touch && p.x>2300 && p.x<2600 && p.y>1600 &&p.y<2500){
    depot();
    testouch=true;
    delay(100);
    }
    if(touch && p.x>2300 && p.x<2600 && p.y>2700 &&p.y<3800){
    testouch=true;
    virement();
    delay(100);
    }
    if(touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){
    aurevoir();
    delay(100);
    testouch=true;
    }
  }
}
bool Welcome(){
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(100, 10);tft.print("Bienvenue");
  tft.setCursor(10, 110);tft.print("Touchez l'ecran pour");tft.setCursor(10, 135);tft.print("commencer");
  while (!ts.touched()){} // wait for touch
  insertion(); 
  return true;
}
bool insertion(){
  bool testcarte=false;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(100, 10);tft.print("Bienvenue");
  tft.setCursor(10, 110);tft.print("Veuillez scanner votre");tft.setCursor(10, 135);tft.print("carte");
  while (!ts.touched()){} // wait for touch
  carte_id = "71d5f58";
  //carte_id = GetId();
  Serial.println(carte_id);
  AskData(String(carte_id));
  code();
  return true;
}
void AskData(String carte_id){
  while (true){
    sendMsgToSlaveWithConfirmation("DATA");
    sendMsgToSlave(carte_id);
    while (true){
        delay(10);
        if (Serial3.available()>0){
          delay(CommunicationDelay); // Wait for the short message to arrive
          Serial.print("Decrypting cards details");
          while (Serial3.available() > 0) {
            char serialData = Serial3.read();
            carte += String(serialData);
          }
          break;
        }
    }
    Serial.println(carte);
    id = ExtractFieldValue(carte, "A");
    intCarteCode = ExtractFieldValue(carte, "C");
    //stringCarteCode+=String(intCarteCode); //marche pas
    solde = ExtractFieldValue(carte, "S");
    break;
  }
}
bool ModifyData(String carte_id, int newSolde, int newCarteCode){
  while (true){
    sendMsgToSlaveWithConfirmation("MODIFY");
    sendMsgToSlave(carte_id);
    String response;
    while (true){
        delay(10);
        if (Serial3.available()>0){
          delay(CommunicationDelay); // Wait for the short message to arrive
          Serial.print("Sending carte_id to blockchain");
          while (Serial3.available() > 0) {
            char serialData = Serial3.read();
            response += String(serialData);
          }
          break;
        }
    }
    if (response == "OK_CARTE"){
      sendMsgToSlave(String(newSolde));
      response = "";
      while (true){
          delay(10);
          if (Serial3.available()>0){
            delay(CommunicationDelay); // Wait for the short message to arrive
            Serial.print("Sending solde to blockchain");
            while (Serial3.available() > 0) {
              char serialData = Serial3.read();
              response += String(serialData);
            }
            break;
          }
      }
      if (response == "OK_SOLDE"){
        sendMsgToSlave(String(newCarteCode));
        response = "";
        while (true){
            delay(10);
            if (Serial3.available()>0){
              delay(CommunicationDelay); // Wait for the short message to arrive
              Serial.print("Sending solde to blockchain");
              while (Serial3.available() > 0) {
                char serialData = Serial3.read();
                response += String(serialData);
              }
              break;
            }
        }
        if (response == "OK_CODE"){
          return true;
      } else {
        return false;
      } 
      } else {
        return false;
      }  
    } else {
      return false;
    }  
    break;
  }
}

void virement(){
  bool testouch=false;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(10, 110);tft.print("Y a pas l'option");
  tft.setTextColor(ILI9341_WHITE);
  tft.fillRoundRect(5,195,94,24,10,ILI9341_BLACK);tft.setCursor(10, 200);tft.print("Annuler");
  while(testouch==false){
    boolean touch = ts.touched();
    TS_Point p = ts.getPoint();
    if(touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){
    menu();
    delay(100);
    testouch=true;
    }
  }
}

void retrait(){
  bool testouch=false;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(110, 10);tft.print("Retrait");
  tft.setCursor(10, 30);tft.print("Combien voulez vous \n retirer?");
  tft.setTextColor(ILI9341_WHITE);
  tft.fillRoundRect(10,195,94,24,10,ILI9341_BLACK);tft.setCursor(15, 200);tft.print("Annuler");
  tft.fillRoundRect(205,195,94,24,10,ILI9341_BLACK);tft.setCursor(210, 200);tft.print("Valider");
  tft.fillRoundRect(10,95,45,24,10,ILI9341_BLACK);tft.setCursor(15, 100);tft.print("10$");
  tft.fillRoundRect(85,95,45,24,10,ILI9341_BLACK);tft.setCursor(90, 100);tft.print("20$");
  tft.fillRoundRect(165,95,45,24,10,ILI9341_BLACK);tft.setCursor(170, 100);tft.print("50$");
  tft.fillRoundRect(240,95,55,24,10,ILI9341_BLACK);tft.setCursor(245, 100);tft.print("100$");
  tft.fillRoundRect(70,135,165,24,10,ILI9341_BLACK);tft.setCursor(75, 140);tft.print("Autre montant");
  int soldeTemp=0;
  while(testouch==false){
    boolean touch = ts.touched();
    TS_Point p = ts.getPoint();
    tft.fillRect(50, 180, 140, 60, ILI9341_WHITE);
  tft.setTextColor(ILI9341_DARKGREEN);
  tft.setCursor(10, 180);tft.print("X = ");tft.print(p.x);
  tft.setCursor(10, 210);tft.print("Y = ");tft.print(p.y);
    if(touch && p.x>1200 && p.x<2200 && p.y>200 &&p.y<800){
    soldeTemp=10;
    Serial.println("10$");
    delay(100);
    }
    if(touch && p.x>1200 && p.x<2200 && p.y>1200 &&p.y<1800){
    soldeTemp=20;
    Serial.println("20$");
    delay(100);
    }
    if(touch && p.x>1200 && p.x<2200 && p.y>2200 &&p.y<2800){
    soldeTemp=50;
    Serial.println("50$");
    delay(100);
    }
    if(touch && p.x>1200 && p.x<2200 && p.y>3200 &&p.y<3800){
    soldeTemp=100;
    Serial.println("100$");
    delay(100);
    }
    if(touch && p.x>2300 && p.x<2700 && p.y>1100 &&p.y<3000){
    autreMontant();
    delay(100);
    }
    if(touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){ //annuler
    menu();
    delay(100);
    testouch=true;
    }
    if(touch && p.x>3200 && p.x<3600 && p.y>2700 &&p.y<3700){ // valider
    solde-=soldeTemp;
    Serial.println(soldeTemp);
    menu();
    delay(100);
    testouch=true;
    }
  }
}
void depot(){
  bool testouch=false;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(110, 10);tft.print("Retrait");
  tft.setCursor(10, 30);tft.print("Combien voulez vous \n déposer?");
  tft.setTextColor(ILI9341_WHITE);
  tft.fillRoundRect(10,195,94,24,10,ILI9341_BLACK);tft.setCursor(15, 200);tft.print("Annuler");
  tft.fillRoundRect(205,195,94,24,10,ILI9341_BLACK);tft.setCursor(210, 200);tft.print("Valider");
  tft.fillRoundRect(10,95,45,24,10,ILI9341_BLACK);tft.setCursor(15, 100);tft.print("10$");
  tft.fillRoundRect(85,95,45,24,10,ILI9341_BLACK);tft.setCursor(90, 100);tft.print("20$");
  tft.fillRoundRect(165,95,45,24,10,ILI9341_BLACK);tft.setCursor(170, 100);tft.print("50$");
  tft.fillRoundRect(240,95,55,24,10,ILI9341_BLACK);tft.setCursor(245, 100);tft.print("100$");
  tft.fillRoundRect(70,135,165,24,10,ILI9341_BLACK);tft.setCursor(75, 140);tft.print("Autre montant");
  int soldeTemp=0;
  while(testouch==false){
    boolean touch = ts.touched();
    TS_Point p = ts.getPoint();
    tft.fillRect(50, 180, 140, 60, ILI9341_WHITE);
  tft.setTextColor(ILI9341_DARKGREEN);
  tft.setCursor(10, 180);tft.print("X = ");tft.print(p.x);
  tft.setCursor(10, 210);tft.print("Y = ");tft.print(p.y);
    if(touch && p.x>1200 && p.x<2200 && p.y>200 &&p.y<800){
    soldeTemp=10;
    Serial.println("10$");
    delay(100);
    }
    if(touch && p.x>1200 && p.x<2200 && p.y>1200 &&p.y<1800){
    soldeTemp=20;
    delay(100);
    }
    if(touch && p.x>1200 && p.x<2200 && p.y>2200 &&p.y<2800){
    soldeTemp=50;
    delay(100);
    }
    if(touch && p.x>1200 && p.x<2200 && p.y>3200 &&p.y<3800){
    soldeTemp=100;
    delay(100);
    }
    if(touch && p.x>2300 && p.x<2700 && p.y>1100 &&p.y<3000){
    autreMontant();
    delay(100);
    }
    if(touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){ //annuler
    menu();
    delay(100);
    testouch=true;
    }
    if(touch && p.x>3200 && p.x<3600 && p.y>2700 &&p.y<3700){ // valider
    depot_confirmation(soldeTemp);
    delay(100);
    testouch=true;
    }
  }
}
void depot_confirmation(int soldeTemp){
  bool testouch=false;
  int montant=soldeTemp;
  int nbBillet=soldeTemp/10;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(110, 10);tft.print("Depot");
  tft.setCursor(10, 30);tft.print("Veuillez placer vos \n billets dans l'enveloppe");
  tft.setCursor(10, 90);tft.print("Billets :");
  tft.setTextColor(ILI9341_WHITE);tft.fillRoundRect(155,85,94,24,10,ILI9341_BLACK);tft.setCursor(160, 90);tft.print(nbBillet);
  tft.setTextColor(ILI9341_BLACK);tft.setCursor(10, 150);tft.print("Montant :");
  tft.setTextColor(ILI9341_WHITE);tft.fillRoundRect(155,145,94,24,10,ILI9341_BLACK);tft.setCursor(160, 150);tft.print(montant);tft.print("$");
  tft.fillRoundRect(5,195,94,24,10,ILI9341_BLACK);tft.setCursor(10, 200);tft.print("Annuler");
  tft.fillRoundRect(205,195,94,24,10,ILI9341_BLACK);tft.setCursor(210, 200);tft.print("Valider");
  while(testouch==false){
    boolean touch = ts.touched();
    TS_Point p = ts.getPoint();
    if(touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){
    menu();
    delay(100);
    testouch=true;
    }
    if(touch && p.x>3200 && p.x<3600 && p.y>2700 &&p.y<3700){
    while (!ModifyData(carte_id, solde+montant, intCarteCode)){}
      solde+=montant;
    menu();
    delay(100);
    testouch=true;
    }
  }
}
bool code(){
  bool testcode=false;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(10, 10);tft.print("Bonjour ");tft.print(carte_id);
  tft.setCursor(10, 35);tft.print("Veuillez rentrer votre"),tft.setCursor(10, 60);tft.print("code sur le digicode");
  tft.setTextColor(ILI9341_WHITE);
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);
  //tft.fillRoundRect(5,195,94,24,10,ILI9341_BLACK);tft.setCursor(10, 200);tft.print("Valider");
  tft.setTextSize(4);
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);tft.setCursor(70, 110);tft.print("_");
  //String code; 
  pad = "";
  getNumber();
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);tft.setCursor(70, 110);tft.print("*");
  delay(1000);
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);tft.setCursor(70, 110);tft.print("* _");
  getNumber();
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);tft.setCursor(70, 110);tft.print("* *");
  delay(1000);
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);tft.setCursor(70, 110);tft.print("* * _");
  getNumber();
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);tft.setCursor(70, 110);tft.print("* * *");
  delay(1000);
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);tft.setCursor(70, 110);tft.print("* * * _");
  Serial.println(pad);
  getNumber();
  Serial.println("apres getn umber, avant scree:code: "); // pas toucher cette zone ç tient du miracle
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);tft.setCursor(70, 110);tft.print("* * * *");  tft.setTextSize(2);  tft.fillRoundRect(5,195,94,24,10,ILI9341_BLACK);tft.setCursor(10, 200);tft.print("Valider");



  bool boutonTouche = false;
  while (!boutonTouche){
    while(!ts.touched()){
      if (ts.getPoint().x>3200 && ts.getPoint().x<3600 && ts.getPoint().y>450 && ts.getPoint().y<1500)
        boutonTouche = true;
    }
  }
  Serial.println("Bouton touché");
  //String code2=String(code);
  Serial.println(pad);
  Serial.println(String(intCarteCode));
  if (pad.toInt() == intCarteCode){ //bug
    Serial.println("Les codes correspondent");
    menu();
    // check screen cadenas ?
    return true;
  } else {
    Serial.println("Les codes ne correspondent pas ");
    // check screen + error
    return false;
  }
}
unsigned long aurevoir(){
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(10, 110);tft.print("Au revoir !");
  delay(2000);
}

void autreMontant(){
  bool testouch=false;
  int montant=10;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(110, 10);tft.print("Retrait");
  tft.setCursor(10, 30);tft.print("Combien voulez vous \n retirer?");
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.fillRoundRect(215,105,30,30,10,ILI9341_BLACK);tft.setCursor(222, 110);tft.print("+");
  tft.fillRoundRect(75,105,30,30,10,ILI9341_BLACK);tft.setCursor(82, 110);tft.print("-");
  tft.setTextSize(2);
  tft.fillRoundRect(5,195,94,24,10,ILI9341_BLACK);tft.setCursor(10, 200);tft.print("Annuler");
  tft.fillRoundRect(205,195,94,24,10,ILI9341_BLACK);tft.setCursor(210, 200);tft.print("Valider");
  while(testouch==false){
    tft.fillRoundRect(115,105,94,30,10,ILI9341_BLACK);tft.setCursor(120, 110);tft.print(montant);tft.print("$");
    boolean touch = ts.touched();
    TS_Point p = ts.getPoint();
    if(touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){
    retrait();
    delay(100);
    testouch=true;
    }
    if(touch && p.x>3200 && p.x<3600 && p.y>2700 &&p.y<3700){
    solde=solde-montant;
    menu();
    delay(100);
    testouch=true;
    }
    if(touch && p.x>1800 && p.x<2200 && p.y>1100 &&p.y<1500){
    montant=montant-10;
    delay(100);
    }
    if(touch && p.x>1800 && p.x<2200 && p.y>2600 &&p.y<3000){
    montant=montant+10;
    delay(100);
    }
    delay(100);
  }
}
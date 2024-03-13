#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
int ExtractFieldValue(String dataReceived, String fieldName) {
  const size_t capacity = JSON_OBJECT_SIZE(4) + 60; //ici il y a 4 valeurs: id, carte_id, solde, carte_code >>> à ajuster en fonction des données reçus
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
// Main
String carte;
int carte_code;
int solde;
String carte_id;
String id;


#include <XPT2046_Touchscreen.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <SPI.h>

#define CS_PIN_TOUCH_SCREEN  47 //SS + CS = Slave Select
#define TFT_DC  49
#define TFT_CS 53
#define RST_PIN 48

// RST identique pour RST ECRAN & RFID

#include <MFRC522.h> //RFID
#define CS_PIN_RFID 43 //Use D before for the WIFI CARD
#define RST_PIN_RFID 41 //Use D before for the WIFI CARD

MFRC522 rfid(CS_PIN_RFID, RST_PIN_RFID);

//communication
//#include <SoftwareSerial.h>
#define txPin 8 // mois d'anniversaire de Arthur & Martin
#define rxPin 7 // jour d'anniversaire de Martin & Arthur
int CommunicationDelay=1000;
//int ConsoleRefreshDelay=1000;
void COMSetup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial3.begin(9600);
  //Serial.begin(115200);
}
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
const int Ligne = 4;
const int Colonne = 4;

char hexaBouton[Ligne][Colonne] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

int lignesPins[Ligne] = {22, 24, 26, 28}; // Broches pour les lignes (D0 à D3)
int colonnesPins[Colonne] = {30, 32, 34, 36};
String getNumber(){
  char boutonPresse;
  bool play = true;
  while (play){
    for (int i = 0; i < Colonne; i++) {
      // Activer la colonne en cours
      digitalWrite(colonnesPins[i], LOW);
      
      // Lire les lignes
      for (int j = 0; j < Ligne; j++) {
        if (digitalRead(lignesPins[j]) == LOW) {
          // Un bouton a été pressé dans la ligne j et la colonne i
          boutonPresse = hexaBouton[j][i];
          //Serial.print("\nbutton: "+boutonPresse);
          play = false;
          
          // Attendez que le bouton soit relâché
          while (digitalRead(lignesPins[j]) == LOW) {
            delay(10);
          }
        }
      }

      // Désactiver la colonne en cours
      digitalWrite(colonnesPins[i], HIGH);
    }
  }
  Serial.println(String(boutonPresse));
  return String(boutonPresse);
}





int soldeTemp=0;
XPT2046_Touchscreen ts(CS_PIN_TOUCH_SCREEN);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, RST_PIN);
void setup() {
  for (int i = 0; i < Colonne; i++) {
    pinMode(colonnesPins[i], OUTPUT);
    digitalWrite(colonnesPins[i], HIGH);
  }
  // Initialiser les broches de ligne avec une résistance de pull-up interne
  for (int i = 0; i < Ligne; i++) {
    pinMode(lignesPins[i], INPUT_PULLUP);
  }

  
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(CS_PIN_RFID, OUTPUT);
  digitalWrite(CS_PIN_RFID, HIGH);

  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  pinMode(CS_PIN_TOUCH_SCREEN, OUTPUT);
  digitalWrite(CS_PIN_TOUCH_SCREEN, HIGH);
  tft.begin();
  ts.begin();
  //Serial.begin(115200);
  tft.setRotation(1);
  ts.setRotation(2);
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextSize(2);

  Serial.println("La carte à démarré !");

  COMSetup();
}

void loop() {

  /*boolean istouched = ts.touched();
  TS_Point p = ts.getPoint();
  tft.fillRect(50, 180, 140, 60, ILI9341_WHITE);
  tft.setTextColor(ILI9341_DARKGREEN);
  tft.setCursor(10, 180);tft.print("X = ");tft.print(p.x);
  tft.setCursor(10, 210);tft.print("Y = ");tft.print(p.y);
  if(istouched){
    insertion();
  }
  istouched=false;
  delay(100);*/
  insertion();

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
void insertion(){
  bool testcarte=false;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(100, 10);tft.print("Bienvenue");
  tft.setCursor(10, 110);tft.print("Veuillez scanner votre");tft.setCursor(10, 135);tft.print("carte");
  carte_id = GetId();
  Serial.println(carte_id);
  AskData(String(carte_id));
  //delay(10000);
  code();
}
String AskData(String carte_id){
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
    carte_code = ExtractFieldValue(carte, "C");
    solde = ExtractFieldValue(carte, "S");
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
  while(testouch==false){
    boolean touch = ts.touched();
    TS_Point p = ts.getPoint();
    soldeTemp=0;
    if(touch && p.x>500 && p.x<900 && p.y>1600 &&p.y<1900){
    soldeTemp=soldeTemp+10;
    delay(100);
    }
    if(touch && p.x>1300 && p.x<1800 && p.y>1600 &&p.y<1900){
    soldeTemp=soldeTemp+20;
    delay(100);
    }
    if(touch && p.x>2200 && p.x<2700 && p.y>1600 &&p.y<1900){
    soldeTemp=soldeTemp+50;
    delay(100);
    }
    if(touch && p.x>3100 && p.x<3600 && p.y>1600 &&p.y<1900){
    soldeTemp=soldeTemp+100;
    delay(100);
    }
    if(touch && p.x>2300 && p.x<2700 && p.y>1100 &&p.y<3000){
    autreMontant();
    delay(100);
    }
    if(touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){
    menu();
    delay(100);
    testouch=true;
    }
    if(touch && p.x>3200 && p.x<3600 && p.y>2700 &&p.y<3700){
    solde=solde-soldeTemp;
    menu();
    delay(100);
    testouch=true;
    }
  }
}
void depot(){
  bool testouch=false;
  int montant=0;
  int nbBillet=0;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(110, 10);tft.print("Depot");
  tft.setCursor(10, 30);tft.print("Veuillez inserer vos \n billets");
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
    menu();
    delay(100);
    testouch=true;
    }
  }
}
void code(){
  bool testcode=false;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(10, 10);tft.print("Bonjour ");tft.print(carte_id);
  tft.setCursor(10, 35);tft.print("Veuillez rentrer votre"),tft.setCursor(10, 60);tft.print("code sur le digicode");
  tft.setTextColor(ILI9341_WHITE);
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);
  tft.fillRoundRect(5,195,94,24,10,ILI9341_BLACK);tft.setCursor(10, 200);tft.print("Valider");
  String code = getNumber();
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);tft.setTextSize(4);tft.setCursor(70, 110);tft.print("*");
  code += getNumber();
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);tft.setCursor(70, 110);tft.print("* *");
  code += getNumber();
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);tft.setCursor(70, 110);tft.print("* * *");
  code += getNumber();
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);tft.setCursor(70, 110);tft.print("* * * *");tft.setTextSize(2);

  Serial.println(code);
  Serial.println(String(carte_code));
  if (String(code) == String(carte_code)){ //bug
    Serial.println("Les codes correspondent");
    
  } else {
    Serial.println("Les codes ne correspondent pas ");
  }
  delay(5000);

  
  // base arduino wifi
  while(testcode==false){
    boolean touch = ts.touched();
    TS_Point p = ts.getPoint();
    if (touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){
      menu();
    }
  }
}
unsigned long aurevoir(){
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(10, 110);tft.print("Au revoir !");
  delay(2000);
  insertion();
  menu();
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
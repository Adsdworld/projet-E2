#include <SoftwareSerial.h>
#define rxPin 22
#define txPin 23
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#define CS_PIN  47
#define TFT_DC  49
#define TFT_CS 53
#define TFT_RST 48
XPT2046_Touchscreen ts(CS_PIN);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
int CommunicationDelay=1000;
int ConsoleRefreshDelay=1000;
int SlaveTimeout=60000;
bool dansMenu=false;
unsigned long solde=65536;
unsigned long Timeout;
SoftwareSerial mySerial (rxPin, txPin);

void sendMsgToMaster(String message) {
  mySerial.print(message);
  Serial.println("Un message à été envoyé:"+message);
  delay(CommunicationDelay*3);
}
void sendKeepAlive() {
  mySerial.print("KA");
  Serial.print("Keep alive");
}
String readMsgFromMaster() {
   String receivedMessage = "";
  Serial.println("\n***Waiting for data available from Master");
  while (!mySerial.available()) {

  }
  Serial.print("New data available from Master");
  delay(CommunicationDelay/3); // Wait for the short message to arrive
  while (mySerial.available() > 0) {
    char serialData = mySerial.read();
    receivedMessage += String(serialData);
  }
  Serial.println("\n***Message reçu du Master:"+receivedMessage+"\n");
  return receivedMessage;
}
/*================================================================================
      Digicode
=================================================================================*/
const int Ligne = 4;
const int Colonne = 4;
char hexaBouton[Ligne][Colonne] = {
  {'1', '2', '3', 'a'},
  {'4', '5', '6', 'b'},
  {'7', '8', '9', 'c'},
  {'*', '0', '#', 'd'}
};
int lignesPins[Ligne] = {30, 31, 32, 33}; // Broches pour les lignes (D0 à D3)
int colonnesPins[Colonne] = {34, 35, 36, 37}; // Broches pour les colonnes (D4 à D7)
//int lignesPins[Ligne] = {D7, D6, D5, D4}; // Broches pour les lignes (D0 à D3)
//int colonnesPins[Colonne] = {D3, D2, D1, D0}; // Broches pour les colonnes (D4 à D7)
String Digicode(){
  String code="";
  String a="";
  while (code.length() < Ligne) {
    //Serial.println("1 check effectué");
    for (int i = 0; i < Colonne; i++) {
      // Activer la colonne en cours
      digitalWrite(colonnesPins[i], LOW);
      // Lire les lignes
      for (int j = 0; j < Ligne; j++) {
        if (digitalRead(lignesPins[j]) == LOW) {
          // Un bouton a été pressé dans la ligne j et la colonne i
          char boutonPresse = hexaBouton[j][i];
          if (boutonPresse=="a"||boutonPresse=="b"||boutonPresse=="c"||boutonPresse=="d"){
            delay(100);
          }
          if (boutonPresse=="*"){

          }
          if (boutonPresse=="#"){
            
          }
          code+=boutonPresse;
          a="";
          for (int k = 1; k <= code.length(); k++) {
            a += "#";
          }
          Serial.println(a);
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
  return code;
}
void setup() {
  delay(5000);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(115200);
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  // Initialiser les broches de colonne en sortie avec HIGH
  for (int i = 0; i < Colonne; i++) {
    pinMode(colonnesPins[i], OUTPUT);
    digitalWrite(colonnesPins[i], HIGH);
  }
  // Initialiser les broches de ligne avec une résistance de pull-up interne
  for (int i = 0; i < Ligne; i++) {
    pinMode(lignesPins[i], INPUT_PULLUP);
  }
  tft.begin();
  ts.begin();
  tft.setRotation(1);
  ts.setRotation(2);
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextSize(2);
}

void loop() {
  String code=Digicode();
  Serial.print(code);
  msgEcran(code);
  /*String receivedMessage = readMsgFromMaster();
  if (receivedMessage == "START") {
    sendMsgToMaster("OK");
    Serial.println("animation de l'écran");
    msgEcran("Connection started");
    sendMsgToMaster("STARTED");
  }
  if (receivedMessage == "MENU" or dansMenu==true) {
    sendMsgToMaster("OK");
    dansMenu=true;
    String user=readMsgFromMaster();
    sendMsgToMaster("OK");
    menu();
    sendMsgToMaster("MENUED");
  }*/
}
unsigned long msgEcran(String message){
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(10, 110);tft.print(message);
  delay(2000);
}
String InstantreadMsgFromMaster() {
  String receivedMessage = "";
  //Serial.println("\n***Checking for data available from Master");
  if (mySerial.available()>0){
    Serial.print("New data available from Master");
    delay(CommunicationDelay); // Wait for the short message to arrive
    while (mySerial.available() > 0) {
      char serialData = mySerial.read();
      receivedMessage += String(serialData);
    }
    Serial.println("\n***Message reçu du Master:"+receivedMessage+"\n");
    return receivedMessage;
  }
  //Serial.println("\n***Checked");
  return "null";
}
//==============================================================================
//Fonction Ecran
//==============================================================================
void menu(){
  bool testouch=false;
  Timeout=millis()+(SlaveTimeout/6);
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
    //Serial.println("Current millis:"+String(millis())+" Timeout:"+String(Timeout));
    if(Timeout<millis()){
      sendKeepAlive();
      //delay(1000);
      Timeout=Timeout+SlaveTimeout/6;
    }
    boolean touch = ts.touched();
    TS_Point p = ts.getPoint();
    //Serial.print("x:"+String(p.x)+"y:"+String(p.y));
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
void virement(){
  bool testouch=false;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(10, 110);tft.print("Y a pas l'option");
  tft.setTextColor(ILI9341_WHITE);
  tft.fillRoundRect(5,195,94,24,10,ILI9341_BLACK);tft.setCursor(10, 200);tft.print("Annuler");
  while(testouch==false){
    if(Timeout<millis()){
      sendKeepAlive();
      Timeout=Timeout+SlaveTimeout/6;
    }
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
    if(Timeout<millis()){
      sendKeepAlive();
      Timeout=Timeout+SlaveTimeout/6;
    }
    boolean touch = ts.touched();
    TS_Point p = ts.getPoint();
    if(touch && p.x>500 && p.x<900 && p.y>1600 &&p.y<1900){
    solde=solde+10;
    delay(100);
    }
    if(touch && p.x>1300 && p.x<1800 && p.y>1600 &&p.y<1900){
    solde=solde+20;
    delay(100);
    }
    if(touch && p.x>2200 && p.x<2700 && p.y>1600 &&p.y<1900){
    solde=solde+50;
    delay(100);
    }
    if(touch && p.x>3100 && p.x<3600 && p.y>1600 &&p.y<1900){
    solde=solde+100;
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
    if(touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){
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
  tft.setCursor(110, 10);tft.print("Depot");
  tft.setCursor(10, 30);tft.print("Veuillez inserer vos \n billets");
  tft.setTextColor(ILI9341_WHITE);
  tft.fillRoundRect(5,195,94,24,10,ILI9341_BLACK);tft.setCursor(10, 200);tft.print("Annuler");
  tft.fillRoundRect(205,195,94,24,10,ILI9341_BLACK);tft.setCursor(210, 200);tft.print("Valider");
  while(testouch==false){
    if(Timeout<millis()){
      sendKeepAlive();
      Timeout=Timeout+SlaveTimeout/6;
    }
    boolean touch = ts.touched();
    TS_Point p = ts.getPoint();
    if(touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){
    menu();
    delay(100);
    testouch=true;
    }
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
  tft.fillRoundRect(115,105,94,30,10,ILI9341_BLACK);tft.setCursor(120, 110);tft.print(montant);tft.print("$");
  tft.setTextSize(2);
  tft.fillRoundRect(5,195,94,24,10,ILI9341_BLACK);tft.setCursor(10, 200);tft.print("Annuler");
  tft.fillRoundRect(205,195,94,24,10,ILI9341_BLACK);tft.setCursor(210, 200);tft.print("Valider");
  while(testouch==false){
    boolean touch = ts.touched();
    TS_Point p = ts.getPoint();
    if(touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){
    retrait();
    delay(100);
    testouch=true;
    }
    if(touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){
    retrait();
    delay(100);
    testouch=true;
    }
    if(touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){
    montant=montant-10;
    autreMontant();
    delay(100);
    }
    if(touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){
    montant=montant+10;
    autreMontant();
    delay(100);
    }
  }
}

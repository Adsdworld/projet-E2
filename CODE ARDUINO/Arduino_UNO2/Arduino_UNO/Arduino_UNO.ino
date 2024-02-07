/****************************************************************************************************************************************
* TOUT SAUF ECRAN:                                                                                                                                 *
*   Contient les Variables essentiels.                                                                                                  *
****************************************************************************************************************************************/
int TimeoutMsgResponse=1000;
int TimeoutArduinoUno=20000;
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#define CS_PIN  7
#define TFT_DC  9
#define TFT_CS 10
#define TFT_RST 8
XPT2046_Touchscreen ts(CS_PIN);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
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

void setup() {
  Serial.begin(115200);
  Serial.print("DEMARRER");
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  tft.begin();
  ts.begin();
  tft.setRotation(3);
  ts.setRotation(4);
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextSize(2);
}

void loop() {
  if (0<Serial.available()){
    Serial.flush();
    String StringBuilder="";
    delay(TimeoutMsgResponse); //laisser un peu de temps pour que le court message arrive
    while(Serial.available()>0){
      char Serialdata =Serial.read();
      StringBuilder+=String(Serialdata);
    }
    if (StringBuilder==String("START")){//affiche une animation cool de démarrage
      Serial.print("OK");
      Start();
      Serial.print("STARTED");
    }
    else if(StringBuilder==String("CONNECTING")){
      Serial.print("OK");
      Connecting();
    }
    else if (StringBuilder==String("WELCOME")){//affiche une animation cool de démarrage
      Serial.print("OK");
      Welcome();
      Serial.print("TOUCH");
      menu();
    }
    else if(StringBuilder==String("RFID")){
      Serial.print("OK");
      RFID();
    }
    else if(StringBuilder.charAt(0)=='D'){
      Serial.print("OK");
      DigiCode(StringBuilder);
    }
    else if(StringBuilder==String("DATABASE")){
      Serial.print("OK");
      DataBase();
    }
    else if(StringBuilder==String("AUREVOIR")){
      Serial.print("OK");
      aurevoir("AUREVOIR MSG");
    }
  }
}

void Start(){
  int Timeout=millis()+TimeoutArduinoUno;
  while(true){
    while(millis()<Timeout){
      //Some screen animations, do not exceed TimeoutArduinoUno or découper en petite animations et vérifier le timetout entre chaques pour svoir si oui ou non envoyer un KeepAlive
      //tft.fillCircle(100,100,50,ILI9341_RED);
      aurevoir("Start");
      return;
    }
    Serial.print("KA");
  }
}
void Connecting(){
  while(true){
    //3 fast wifi connecting screen animation
    if (ReadInstantMsgFromArduino()=="CONNECTED"){
      return;
    }
  }
}
void Welcome(){
  int Timeout=millis()+TimeoutArduinoUno;
  aurevoir("WELCOME Touch screen please");
  while(true){
    while(millis()<Timeout){
      if (ts.touched()){
        aurevoir("TOUCHED");
        return;
      }
    }
    Serial.print("KA");
  }
}
void RFID(){
  while(true){
    //animations courtes?
    if (ReadInstantMsgFromArduino()=="RFIDED"){
      return;
    }
  }
}
void DigiCode(String Message){
  if (Message=="D0"){
    //_ _ _ _
  }
  else if (Message=="D1"){
   //* _ _ _   
  }
  else if (Message=="D2"){
    // * * _ _
  }
  else if (Message=="D3"){
    // * * * _
  }
  else if (Message=="D4"){
    // * * * * use '#' to validate code
  }
}
void DataBase(){
  while(true){
    //animations courtes? rond qui tourne
    if (ReadInstantMsgFromArduino()=="DATABASED"){
      return;
    }
  }
}
void insertion(){
  bool testcarte=false;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(100, 10);tft.print("Bienvenue");
  tft.setCursor(10, 110);tft.print("Veuillez scanner votre");tft.setCursor(10, 135);tft.print("carte");

  delay(2000);
}

unsigned long aurevoir(String message){
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(10, 110);tft.print(message);
  delay(2000);
}

/****************************************************************************************************************************************
* ECRAN:                                                                                                                                 *
*   Contient les Variables essentiels.                                                                                                  *
****************************************************************************************************************************************/
//fonctions avec des animations stylés:
// cercle qui grandit en fonction du point de contact
// cercle qui apparait à l'endroit du contact 
// animation style win7 qui démarre
// fonctions direction de message 

// à coté des fonction spéciques style
// écrant retrait depot avec inclure le contact + des fonction stylés
void menu(){
  bool testouch=false;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(50, 10);tft.print("Votre solde est de ");
  tft.setCursor(140, 40);tft.print("solde");tft.print("$");
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
    aurevoir("EXIT !");
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
  tft.setTextColor(ILI9341_WHITE);
  tft.fillRoundRect(10,195,94,24,10,ILI9341_BLACK);tft.setCursor(15, 200);tft.print("Annuler");
  tft.fillRoundRect(205,195,74,24,10,ILI9341_BLACK);tft.setCursor(210, 200);tft.print("Finir");
  tft.fillRoundRect(10,105,45,24,10,ILI9341_BLACK);tft.setCursor(15, 110);tft.print("10$");
  tft.fillRoundRect(85,105,45,24,10,ILI9341_BLACK);tft.setCursor(90, 110);tft.print("20$");
  tft.fillRoundRect(165,105,45,24,10,ILI9341_BLACK);tft.setCursor(170, 110);tft.print("50$");
  tft.fillRoundRect(240,105,55,24,10,ILI9341_BLACK);tft.setCursor(245, 110);tft.print("100$");
  while(testouch==false){
    boolean touch = ts.touched();
    TS_Point p = ts.getPoint();
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
  tft.setTextColor(ILI9341_WHITE);
  tft.fillRoundRect(5,195,94,24,10,ILI9341_BLACK);tft.setCursor(10, 200);tft.print("Annuler");
  tft.fillRoundRect(205,195,94,24,10,ILI9341_BLACK);tft.setCursor(210, 200);tft.print("Finir");
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
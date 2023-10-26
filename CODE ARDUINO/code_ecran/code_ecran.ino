#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <XPT2046_Touchscreen.h>
#include <SPI.h>

#define CS_PIN  7
#define TFT_DC  9
#define TFT_CS 10
#define TFT_RST 8

int solde = 32767;
char nom[]="Lilian";
XPT2046_Touchscreen ts(CS_PIN);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
//MFRC522 rfid(RFID_CS, RFID_RST);
//byte nuidPICC[4];
void setup() {
  //Serial.begin(9600);
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  //pinMode(RFID_CS, OUTPUT);
  //digitalWrite(RFID_CS, HIGH);
  //SPI.begin();
  tft.begin();
  ts.begin();
  tft.setRotation(3);
  ts.setRotation(4);
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextSize(2);
  // Init SPI bus
  

  // Init MFRC522
  //rfid.PCD_Init();
}
/*String GetId() {
  String id = "";

  while (!rfid.PICC_IsNewCardPresent()) {
    //delay(50);
    //Serial.print("on attends");
    //return id;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    String id;
    for (byte i = 0; i < 4; i++) {
      id += String(rfid.uid.uidByte[i], HEX);
    }

    Serial.println("Un badge est détecté. L'UID du tag est: ");
    Serial.println(id);

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  if(id==0000 || id=="0000"){
      return GetId();
    }else{
      return id;
    }
  }
  return GetId();
}*/
void loop() {
  //String RFID_id = GetId();
  //Serial.println("Une carte à été reconnue!");
  TS_Point p = ts.getPoint();
  tft.setTextColor(ILI9341_DARKGREEN);
  tft.setCursor(10, 180);tft.print("X = ");tft.print(p.x);
  tft.setCursor(10, 210);tft.print("Y = ");tft.print(p.y);
  


  boolean istouched = ts.touched();
  /*TS_Point p = ts.getPoint();
  tft.fillRect(50, 180, 140, 60, ILI9341_WHITE);
  tft.setTextColor(ILI9341_DARKGREEN);
  tft.setCursor(10, 180);tft.print("X = ");tft.print(p.x);
  tft.setCursor(10, 210);tft.print("Y = ");tft.print(p.y);
  */if(istouched){
    insertion();
  }
  istouched=false;
  delay(100);
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
  while(testcarte==false){
    testcarte=true;
  }
  delay(2000);
  code();
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
void code(){
  bool testcode=false;
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(10, 10);tft.print("Bonjour ");tft.print(nom);
  tft.setCursor(10, 35);tft.print("Veuillez rentrer votre"),tft.setCursor(10, 60);tft.print("code sur le digicode");
  tft.setTextColor(ILI9341_WHITE);
  tft.fillRoundRect(5,195,94,24,10,ILI9341_BLACK);tft.setCursor(10, 200);tft.print("Valider");
  tft.fillRoundRect(65,100,174,54,10,ILI9341_BLACK);tft.setTextSize(4);tft.setCursor(70, 110);tft.print("* * * *");tft.setTextSize(2);
  while(testcode==false){
    boolean touch = ts.touched();
    TS_Point p = ts.getPoint();
    if (touch && p.x>3200 && p.x<3600 && p.y>450 &&p.y<1500){
      menu();
    }
  }
}
void transacannul() {
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(10, 110);tft.print("Transaction annulée");
  delay(2000);
}
unsigned long aurevoir(){
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(10, 110);tft.print("Au revoir !");
  delay(2000);
  insertion();
}

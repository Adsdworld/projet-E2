/****************************************************************************************************************************************
* TOUT SAUF ECRAN:                                                                                                                                 *
*   Contient les Variables essentiels.                                                                                                  *
****************************************************************************************************************************************/
#include <SoftwareSerial.h>
#define rxPin 2
#define txPin 3
int CommunicationDelay=1000;
int ConsoleRefreshDelay=1000;
int SlaveTimeout=60000/3;
SoftwareSerial mySerial (rxPin, txPin);
void COMSetup(){
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(115200);
}
void sendMsgToMaster(String message) {
  mySerial.print(message);
  Serial.println("Une message à été envoyé:"+message);
  delay(CommunicationDelay*4);
}
String readMsgFromMaster() {
  String receivedMessage = "";
  Serial.println("\n***Waiting for data available from Master");
  while (!mySerial.available()) {
  }
  Serial.print("New data available from Master");
  delay(CommunicationDelay); // Wait for the short message to arrive
  while (mySerial.available() > 0) {
    char serialData = mySerial.read();
    receivedMessage += String(serialData);
  }
  Serial.println("\n***Message reçu du Master:"+receivedMessage+"\n");
  return receivedMessage;
}
String InstantreadMsgFromMaster() {
  String receivedMessage = "";
  Serial.println("\n***Checking for data available from Master");
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
  Serial.println("\n***Checked");
  return "null";
}


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
void SCREENSetup(){
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


void setup() {
  COMSetup();
  SCREENSetup();  
}

void loop() {
  String receivedMessage = readMsgFromMaster();
  if (receivedMessage==String("START")){//affiche une animation cool de démarrage
    sendMsgToMaster("OK");
    Start();
    sendMsgToMaster("STARTED");
  }
  else{
    Serial.print("/!\\Message non reconnue/!\\");
  }
}

void Start(){
  int Timeout=millis()+SlaveTimeout;
  while(true){
    //image1
    aurevoir("start img 1");
    delay(5000);
    if (millis()>Timeout){
      sendMsgToMaster("KA");
    }
    //image2
    aurevoir("start img 2");
    return;
  }
}

unsigned long aurevoir(String message){
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(10, 110);tft.print(message);
}
/****************************************************************************************************************************************
* Main:                                                                                                                                 *
*   Contient les Variables essentiels.                                                                                                  *
****************************************************************************************************************************************/
int TimeoutMsgResponse=1000; //Temps d'attente pour recevoir un message
int TimeoutArduinoUno=60000; //Temps d'attente pour comprendre que la UNO est KO
String ArduinoUnoData;       //Message reçu de UNO, peut être positif ou indéchiffrable, sert de stockage tampon pour vérifier si le message est reconnu
String carte_id;
String carte_code;
String solde; //débat String ou int?
bool DebugSerialPrinln=false;
void Shiny(int nb, int ms){
  for (int i=1;i<nb;i++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(ms);
    digitalWrite(LED_BUILTIN, LOW);
    delay(ms);
  }
  return;
}

/****************************************************************************************************************************************
* UART COMMUNICATION:                                                                                                                   *
*   Contient les fonctions de communications                                                                                            *
****************************************************************************************************************************************/
#include <SoftwareSerial.h>
#define rxPin D6
#define txPin D7
int CommunicationDelay=1000;
int ConsoleRefreshDelay=1000;
int SlaveTimeout=60000;
SoftwareSerial mySerial (rxPin, txPin);

void sendMsgToSlave(String message) {
  mySerial.print(message);
  Serial.println("Une message à été envoyé:"+message);
  delay(CommunicationDelay);
}
String readMsgFromSlaveWithTimeout() {
    int Timeout=millis()+SlaveTimeout;
    String receivedMessage = "";
    Serial.println("\n***Waiting for data available from Slave with Timeout:"+String(SlaveTimeout)+"ms");
    while (millis()<Timeout){
      if (mySerial.available()>0){
        Serial.print("New data available from Slave");
        delay(CommunicationDelay); // Wait for the short message to arrive
        while (mySerial.available() > 0) {
          char serialData = mySerial.read();
          receivedMessage += String(serialData);
        }
        Serial.println("\n***Message reçu du Slave:"+receivedMessage+"\n");
        if (receivedMessage=="KA" || receivedMessage=="KAKA"){
          Timeout=millis()+SlaveTimeout;
          Serial.println("Reseting Timeout");
        }
        else{
          return receivedMessage;
        }
      }
  }
  return "KO";
}
void sendMsgToSlaveWithConfirmation(String message) {
  Serial.println("\n***Sending '"+message+"' with confirmation to Slave");
  String receivedMessage = "";
  while (true){
    mySerial.print(message);
    delay(CommunicationDelay*3);
    if (mySerial.available()>0){
      receivedMessage = "";
      delay(CommunicationDelay); // Wait for the short message to arrive
      Serial.print("New data available from Slave");
      while (mySerial.available() > 0) {
        char serialData = mySerial.read();
        receivedMessage += String(serialData);
      }
      if (receivedMessage=="OK"){
        Serial.println("\n***Successfuly sended '"+message+"' with confirmation to Slave");
        return;
      }
    }
  }
}

void COMSetup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(115200);
}

/****************************************************************************************************************************************
* WIFI:                                                                                                                                 *
*   Contient les Librairies et Variables nécessaires pour le Wifi.                                                                      *
****************************************************************************************************************************************/
#include <ESPping.h>
#include <ping.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
WiFiClient client;
#define WIFI_SSID "Redmi Note 8 Pro"
#define WIFI_PASS "asx135bko"
String data="";
String server="projet-e2-eseo.000webhostapp.com";
const char* host = "www.google.com";
void WIFISetup(){
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Connection au wifi");
  while (WiFi.status() != WL_CONNECTED) {
    Shiny(1, 250);
  }
  Serial.println("Connecté ! Adresse IP : ");
  Serial.println(WiFi.localIP());
}



bool IsConnectionActive(){
  if (client.connect(host, 80)) {
    if (client.println("PING / HTTP/1.1\r\nHost: www.google.com\r\n\r\n") > 0) {
    }
    client.stop();
    return true;
  }
  else {
    if (DebugSerialPrinln==true){
      Serial.println("Connection is not active, unable to ping.");
    }
    delay(1000);
    return false;
  }
}
void SendDataToDatabase(String carte_id, String solde, String carte_code){
  if(IsConnectionActive()){
    data = "carte_id=" + carte_id + "&solde=" + solde + "&carte_code=" + carte_code;
    String url = "http://" + server + "/add.php";
    HTTPClient http;
    http.begin(client, url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.POST(data);
    if (httpCode == HTTP_CODE_OK) {
      //Serial.println("Data sent successfully.");
    } 
    else {
      if (DebugSerialPrinln==true){
        Serial.println("HTTP POST FAILED, check wireless connection");
        Serial.println(httpCode);
      }
    }
    http.end();
  }
}
String ReceiveDataFromDatabase(String carte_id){
  if(IsConnectionActive()){
    String url = "http://"+server+"/get_data.php?carte_id=" + carte_id;
    HTTPClient http;
    http.begin(client, url);
    int httpCode = http.GET();
    String data_received;
    if (httpCode == HTTP_CODE_OK) {
      data_received = http.getString();
    }
    else {
      if (DebugSerialPrinln==true){
        Serial.println("HTTP GET FAILED, check wireless connection");
        Serial.println(httpCode);
      }
    }
    http.end();
    return data_received;
  }
  return "null";
}
int ExtractFieldValue(String dataReceived, String fieldName) {
  const size_t capacity = JSON_OBJECT_SIZE(4) + 60; //ici il y a 4 valeurs: id, carte_id, solde, carte_code >>> à ajuster en fonction des données reçus
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, dataReceived);
  if (error) {
    if (DebugSerialPrinln==true){
      Serial.print("Erreur de parsing JSON: ");
      Serial.println(error.c_str());
    }
    return -1; // Valeur par défaut si le parsing échoue
  }
  // Vérifiez si le champ spécifié existe dans le JSON
  if (doc.containsKey(fieldName)) {
    return doc[fieldName].as<int>(); // Renvoie la valeur du champ sous forme d'entier
  }
  else {
    if (DebugSerialPrinln==true){
      Serial.println("Champ non trouvé dans le JSON");
    }
    return -1; // Champ non trouvé, renvoie une valeur par défaut
  }
}

/****************************************************************************************************************************************
* DIGICODE:                                                                                                                             *
*   ???                                                                                                                                 *
****************************************************************************************************************************************/

/****************************************************************************************************************************************
* RFID:                                                                                                                                 *
*   ???                                                                                                                                 *
****************************************************************************************************************************************/

#include <SPI.h> // SPI
#include <MFRC522.h> //RFID
#define SS_PIN D10 //Use D before for the WIFI CARD
#define RST_PIN D9 //Use D before for the WIFI CARD
MFRC522 rfid(SS_PIN, RST_PIN);
void RFIDsetup() {
  SPI.begin();
  rfid.PCD_Init();
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
* DISTRIBUTEUR:                                                                                                                         *
*   ???                                                                                                                                 *
****************************************************************************************************************************************/
void setup() { 
  delay(5000);
  COMSetup(); //mettre en premier pour le Serial.begin
  pinMode(LED_BUILTIN, OUTPUT);
  while(true){
    sendMsgToSlaveWithConfirmation("START");//affiche une animation cool de démarrage
    if (readMsgFromSlaveWithTimeout()==String("STARTED")){ //Attends que l'arduino est finit son animation peut, permet le KeepAlive
      break;
    }
  }
  sendMsgToSlaveWithConfirmation("CONNECTING"); //Affiche un écran dynamique de connection en cours au wifi
  WIFISetup(); 
  sendMsgToSlaveWithConfirmation("CONNECTED");//Envoie la conifirmation de connection
  //RFIDsetup();
}


//envoie du dictionnaire à l'écran et ensuite il utilise la fonction findfiel pour trouver les valeurs
void loop() {
  while(true){
    sendMsgToSlaveWithConfirmation("WELCOME");//Affiche un écran de bienvenue sur le distributeur, tant que le screen n'est pas toucher attendre puis
    ArduinoUnoData=readMsgFromSlaveWithTimeout();
    if (ArduinoUnoData=="TOUCH"){
      break;
    }
  }
  sendMsgToSlaveWithConfirmation("RFID"); //Affiche un écran demandant d'aprocher le badge RFID
  //carte_id=function to get RFID with a TimeOut
  Serial.print("RFIDED");
  sendMsgToSlaveWithConfirmation("D0"); //INteraction écran digicode à develloper
  String UserCode;//=digicode code
  sendMsgToSlaveWithConfirmation("DATABASE");
  data=ReceiveDataFromDatabase(carte_id);
  Serial.print("DATABASED");
  int DatabaseCode=ExtractFieldValue(data, "carte_code");
  if (String(DatabaseCode)==String(UserCode)){
    sendMsgToSlaveWithConfirmation("AUTHENTIFICATED");
    while(true){
      sendMsgToSlaveWithConfirmation("MENU"); //Affiche le menu
      ArduinoUnoData=readMsgFromSlaveWithTimeout();
      if (ArduinoUnoData=="RETRAIT"){

      }
      else if(ArduinoUnoData=="DEPOT"){

      }
      else if(ArduinoUnoData=="VIREMENT"){

      }
      else if(ArduinoUnoData=="LOGOUT"){
        sendMsgToSlaveWithConfirmation("GOODBYE");
        break;
      }
    }
  }
  else{
    sendMsgToSlaveWithConfirmation("DENIED");
  }
}





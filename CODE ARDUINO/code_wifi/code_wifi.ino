/****************************************************************************************************************************************
* Main:                                                                                                                                 *
*   Contient les Variables globales.                                                                                                    *
****************************************************************************************************************************************/
int TimeoutMsgResponse=1000; //Temps d'attente pour recevoir un message
int TimeoutArduinoUno=60000; //Temps d'attente pour comprendre que la UNO est KO
String ArduinoUnoData;       //Message reçu de UNO, peut être positif ou indéchiffrable, sert de stockage tampon pour vérifier si le message est reconnu
String carte_id;
String carte_code;
String solde; //débat String ou int?
bool DebugSerialPrinln=false;

/****************************************************************************************************************************************
* Wifi:                                                                                                                                 *
*   Contient les fonctions de communication avec la Database                                                                                             *
****************************************************************************************************************************************/

//
#include <ESPping.h>
#include <ping.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

WiFiClient client;
#define WIFI_SSID "Redmi Note 8 Pro"
#define WIFI_PASS "asx135bko"
String data="";
String server="projet-e2-eseo.000webhostapp.com";
const char* host = "www.google.com";

void WifiSetup(){
  Serial.println("2/2 Internet ...");
  Serial.println("Connexion au réseau Wifi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connecté à internet !\nAdresse IP : ");
  Serial.println(WiFi.localIP());
}
bool IsConnectionActive(){
  if (client.connect(host, 80)) {
    if (client.println("PING / HTTP/1.1\r\nHost: www.google.com\r\n\r\n") > 0) {
    }
    client.stop();
    return true;
  } else {
    Serial.println("Connection is not active, unable to ping.");
    delay(1000);
    return false;
  }
}
void SendDataToDatabase(String carte_id, String solde, String carte_code){
  if(IsConnectionActive()){
    Serial.println("Arduino>>>Server (active connection>>>execute function)");
    data = "carte_id=" + String(carte_id) + "&solde=" + String(solde) + "&carte_code=" + String(carte_code);

    String url = "http://" + server + "/add.php";
    HTTPClient http;
    http.begin(client, url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.POST(data);
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Data sent successfully.");
    } else {
      Serial.println("HTTP POST FAILED, check wireless connection");
      Serial.println(httpCode);
    }
    if (httpCode == HTTP_CODE_MOVED_PERMANENTLY || httpCode == HTTP_CODE_FOUND) {
      String newUrl = http.header("Location");
      Serial.println(newUrl);
      // Effectuez une nouvelle requête vers la nouvelle URL
      http.end();
      http.begin(client, newUrl);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      httpCode = http.POST(data);
    }
    http.end();
  }
  
  // Faire une vérification en gettant les données et vérifiant si elles match parfaitement.
}
String ReceiveDataFromDatabase(String carte_id){
  if(IsConnectionActive()){
    Serial.println("Server>>>Arduino (active connection>>>execute function)");
    String url = "http://"+server+"/get_data.php?carte_id=" + carte_id;
    HTTPClient http;
    http.begin(client, url);

    int httpCode = http.GET();
    String data_received;
    if (httpCode == HTTP_CODE_OK) {
      data_received = http.getString();
      Serial.println("Data received:");
      Serial.println(data_received);
    } else {
      Serial.println("HTTP GET FAILED, check wireless connection");
      Serial.println(httpCode);
    }
    http.end();
    return data_received;
  }
  return "null";
}
void ModifyDataToDatabase(String carte_id, String solde, String carte_code){
  if(IsConnectionActive()){
    Serial.println("Arduino>>>Server (active connection>>>execute function)");
    data = "carte_id=" + String(carte_id) + "&solde=" + String(solde) + "&carte_code=" + String(carte_code);

    String url = "http://" + server + "/edit.php";
    HTTPClient http;
    http.begin(client, url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.POST(data);
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Data sent successfully.");
    } else {
      Serial.println("HTTP POST FAILED, check wireless connection");
      Serial.println(httpCode);
    }
    if (httpCode == HTTP_CODE_MOVED_PERMANENTLY || httpCode == HTTP_CODE_FOUND) {
      String newUrl = http.header("Location");
      Serial.println(newUrl);
      // Effectuez une nouvelle requête vers la nouvelle URL
      http.end();
      http.begin(client, newUrl);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      httpCode = http.POST(data);
    }
    http.end();
  }
  
  // Faire une vérification en gettant les données et vérifiant si elles match parfaitement.
}

/****************************************************************************************************************************************
* UART COMMUNICATION:                                                                                                                   *
*   Contient les fonctions de communications                                                                                            *
****************************************************************************************************************************************/
#include <SoftwareSerial.h>
#define rxPin D14
#define txPin D15 // j'ai essayé d'inverser ok ici (pas check l'envoie mais reception ok)
int CommunicationDelay=1000;
int ConsoleRefreshDelay=1000;
int SlaveTimeout=60000;
SoftwareSerial mySerial (rxPin, txPin);

void sendMsgToMaster(String message) {
  mySerial.print(message);
  Serial.println("Un message à été envoyé:"+message);
  delay(CommunicationDelay*4);
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
        if (receivedMessage=="KA"){
          Timeout=millis()+SlaveTimeout;
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
    //delay(CommunicationDelay*3);
    delay(10);
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
  Serial.begin(115200);
  Serial.println("\n1/2 Communication ...");
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);
  Serial.println("Communication prête !");
}



void setup() {
  COMSetup();
  WifiSetup();
  Serial.println("La carte Wifi est prête !");
}


void loop() {
  if (0<mySerial.available()){
    //mySerial.flush();
    String StringBuilder="";
    delay(TimeoutMsgResponse); //laisser un peu de temps pour que le court message arrive
    while(mySerial.available()>0){
      char Serialdata =mySerial.read();
      StringBuilder+=String(Serialdata);
    }
    Serial.println(StringBuilder);
    if (StringBuilder == "Message de test pour la communication"){ //Message de test pour la communication
      sendMsgToMaster("OK");
    }
    if (StringBuilder == "DATA"){ // Message pour la récupération des données
      sendMsgToMaster("OK");
      String receivedMessage = "";
      while (true){
        delay(10);
        if (mySerial.available()>0){
          delay(CommunicationDelay); // Wait for the short message to arrive
          Serial.print("Decrypting cards details");
          while (mySerial.available() > 0) {
            char serialData = mySerial.read();
            receivedMessage += String(serialData);
          }
          break;
        }
      }
      sendMsgToMaster(ReceiveDataFromDatabase(String(receivedMessage)));
    }
    if (StringBuilder == "MODIFY"){ // Message pour la récupération des données
      sendMsgToMaster("OK");
      String carte_id = "";
      while (true){
        delay(10);
        if (mySerial.available()>0){
          delay(CommunicationDelay); // Wait for the short message to arrive
          Serial.print("Decrypting cards details");
          while (mySerial.available() > 0) {
            char serialData = mySerial.read();
            carte_id += String(serialData);
          }
          break;
        }
      }
      sendMsgToMaster("OK_CARTE");
      String solde = "";
      while (true){
        delay(10);
        if (mySerial.available()>0){
          delay(CommunicationDelay); // Wait for the short message to arrive
          Serial.print("Decrypting cards details");
          while (mySerial.available() > 0) {
            char serialData = mySerial.read();
            solde += String(serialData);
          }
          break;
        }
      }
      sendMsgToMaster("OK_SOLDE");
      String code = "";
      while (true){
        delay(10);
        if (mySerial.available()>0){
          delay(CommunicationDelay); // Wait for the short message to arrive
          Serial.print("Decrypting cards details");
          while (mySerial.available() > 0) {
            char serialData = mySerial.read();
            code += String(serialData);
          }
          break;
        }
      }
      ModifyDataToDatabase(carte_id, solde, code);
      sendMsgToMaster("OK_CODE");


    }
  }
}





#include <ESPping.h>
#include <ping.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>


//#include <SPI.h> //vérifier si c'est utile


// Variables
WiFiClient client;
#define WIFI_SSID "Redmi Note 8 Pro"
#define WIFI_PASS "asx135bko"
String data="";
String server="projet-e2-eseo.000webhostapp.com";
const char* host = "www.google.com";




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
    Serial.println("Champ non trouvé dans le JSON");
    return -1; // Champ non trouvé, renvoie une valeur par défaut
  }
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
#define rxPin D6
#define txPin D7
int CommunicationDelay=1000;
int ConsoleRefreshDelay=1000;
int SlaveTimeout=60000;
SoftwareSerial mySerial (rxPin, txPin);

void sendMsgToSlave(String message) {
  mySerial.print(message);
  Serial.println("Une message à été envoyé:"+message);
}
String readMsgFromSlave() {
  String receivedMessage = "";
  while (!mySerial.available()) {
    Serial.println("Waiting for data available from Slave");
    delay(ConsoleRefreshDelay*3);
  }
  Serial.println("\nNew data available from Slave");
  delay(CommunicationDelay/4); // Wait for the short message to arrive
  while (mySerial.available() > 0) {
    char serialData = mySerial.read();
    receivedMessage += String(serialData);
  }
  return receivedMessage;
}
void sendMsgToSlaveWithConfirmation(String message) {
  Serial.println("\n***Sending '"+message+"' with confirmation to Slave");
  String receivedMessage = "";
  while (true){
    mySerial.print(message);
    delay(CommunicationDelay*3);
    if (mySerial.available()>0){
      receivedMessage = "";
      delay(CommunicationDelay/4); // Wait for the short message to arrive
      Serial.print("New data available from Slave\n");
      while (mySerial.available() > 0) {
        char serialData = mySerial.read();
        receivedMessage += String(serialData);
      }
      if (receivedMessage=="OK"){
        Serial.println("\n***Successfuly sended '"+message+"' with confirmation to Slave");
        return;
      }else{
        Serial.println("Message reçu: "+receivedMessage);
      }
    }
  }
}

String readMsgFromSlaveWithTimeout() {
    int Timeout=millis()+SlaveTimeout;
    String receivedMessage = "";
    Serial.println("\n***Waiting for data available from Slave with Timeout:"+String(SlaveTimeout)+"ms");
    while (millis()<Timeout){
      //delay(CommunicationDelay);
      //Serial.println(""+String(millis())+" < "+Timeout);
      if (mySerial.available()>0){
        receivedMessage = "";
        Serial.print("New data available from Slave");
        delay(CommunicationDelay/4); // Wait for the short message to arrive
        while (mySerial.available() > 0) {
          char serialData = mySerial.read();
          receivedMessage += String(serialData);
        }
        Serial.println("\n***Message reçu du Slave:"+receivedMessage+"\n");
        if (receivedMessage=="KA"){
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

void setup() {
  delay(5000);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("\nConnection au wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connecté ! Adresse IP : ");
  Serial.println(WiFi.localIP());
}
void loop() {
  while (true){
    sendMsgToSlaveWithConfirmation("MENU");
      String user=ReceiveDataFromDatabase("ARTHUR");
      sendMsgToSlaveWithConfirmation(user);
      String msg = readMsgFromSlaveWithTimeout();
      Serial.print("Retour du read with timeout: "+msg);
      if (msg==String("RFID")){ 
        break;
      }
      if (msg==String("MENUED")){ 
        break;
      }
  }
}



/****************************************************************************************************************************************
* Main:                                                                                                                                 *
*   Contient les Variables globales.                                                                                                    *
****************************************************************************************************************************************/
String carte_id;
String carte_code;
String solde; //débat String ou int?

/****************************************************************************************************************************************
* Wifi:                                                                                                                                 *
*   Contient les fonctions de communication avec la Database                                                                            *
****************************************************************************************************************************************/
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
WiFiClient client;
#define pinReset D2
#define WIFI_SSID "Redmi Note 8 Pro"
#define WIFI_PASS "asx135bko"
String data="";
String server="projet-e2-eseo.000webhostapp.com";


// fonction pour réinitialiser la carte
void WifiReset() {
    digitalWrite(pinReset, LOW);
}


void WifiSetup(){
  pinMode(pinReset, OUTPUT); // pin pour la réinitialisation en sortie
  digitalWrite(pinReset, HIGH); // état pour ne pas reset la carte
  Serial.println("2/2 Internet ...");
  Serial.println("Connexion au réseau Wifi");
  WiFi.begin(WIFI_SSID, WIFI_PASS); // établir la connection au réseau
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connecté à internet !\nAdresse IP : ");
  Serial.println(WiFi.localIP());
}


// recevoir les informations d'une carte
String ReceiveDataFromDatabase(String carte_id){
  Serial.println(String("Identifiant de carte reçu: " + carte_id));
  Serial.println("Server>>>Arduino");
  String url = "http://"+server+"/get_data.php?carte_id=" + carte_id; // requète
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
    WifiReset();
  }
  http.end();
  return data_received;
}


// modifier les informations d'une carte
void ModifyDataToDatabase(String id, String carte_id, String solde, String carte_code){
    Serial.println("Arduino>>>Server");
    data = "id=" + String(id) + "&carte_id=" + String(carte_id) + "&solde=" + String(solde) + "&carte_code=" + String(carte_code);

    String url = "http://" + server + "/edit.php"; // page cible
    HTTPClient http;
    http.begin(client, url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.POST(data);
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Data sent successfully.");
    } else {
      Serial.println("HTTP POST FAILED, check wireless connection");
      Serial.println(httpCode);
      WifiReset();
    }
    http.end();  
}

/****************************************************************************************************************************************
* UART COMMUNICATION:                                                                                                                   *
*   Contient les fonctions de communications                                                                                            *
****************************************************************************************************************************************/
#include <SoftwareSerial.h> // librairie pour créer un sérial RX/TX virtuel
#define rxPin D14
#define txPin D15
int CommunicationDelay=20; //Temps d'attente pour recevoir un message
SoftwareSerial mySerial (rxPin, txPin);


// fonction pour envoyer un message 
void sendMsgToMaster(String message) {
  mySerial.print(message);
  Serial.println("Un message à été envoyé:"+message);
  delay(CommunicationDelay*4);
}


void COMSetup() {
  Serial.begin(115200);
  Serial.println("\n1/2 Communication ...");
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(38400);
  Serial.println("Communication prête !");
}


void setup() {
  COMSetup();
  WifiSetup();
  Serial.println("La carte Wifi est prête !");
}


void loop() {
  if (0<mySerial.available()){
    String StringBuilder="";
    delay(CommunicationDelay); //laisser un peu de temps pour que le court message arrive
    while(mySerial.available()>0){
      char Serialdata =mySerial.read();
      StringBuilder+=String(Serialdata);
    }
    Serial.println(StringBuilder);
    if (StringBuilder == "DATA"){ // Message pour la récupération des données
        sendMsgToMaster("OK");
        String receivedMessage = "";
        while (true){
          delay(10);
          if (mySerial.available()>0){
            delay(CommunicationDelay); // Wait for the short message to arrive
            Serial.println("Decrypting cards details");
            while (mySerial.available() > 0) {
              char serialData = mySerial.read();
              receivedMessage += String(serialData);
            }
            Serial.println(receivedMessage);
            break;
          }
        }
        sendMsgToMaster(ReceiveDataFromDatabase(String(receivedMessage)));
    }
    if (StringBuilder == "MODIFY"){ // Message pour la récupération des données
        sendMsgToMaster("OK");
        String id = "";
        while (true){
          if (mySerial.available()>0){
            delay(CommunicationDelay);
            Serial.print("Decrypting cards details");
            while (mySerial.available() > 0) {
              char serialData = mySerial.read();
              id += String(serialData);
            }
            break;
          }
        }
        Serial.println(id);
        sendMsgToMaster("OK_ID"); // répondre pour le cevoir la prochaine information
        String carte_id = "";
        while (true){
          if (mySerial.available()>0){
            delay(CommunicationDelay);
            Serial.print("Decrypting cards details");
            while (mySerial.available() > 0) {
              char serialData = mySerial.read();
              carte_id += String(serialData);
            }
            break;
          }
        }
        Serial.println(carte_id);
        sendMsgToMaster("OK_CARTE_ID"); // répondre pour le cevoir la prochaine information
        String solde = "";
        while (true){
          if (mySerial.available()>0){
            delay(CommunicationDelay);
            Serial.print("Decrypting cards details");
            while (mySerial.available() > 0) {
              char serialData = mySerial.read();
              solde += String(serialData);
            }
            break;
          }
        }
        Serial.println(solde);
        sendMsgToMaster("OK_SOLDE"); // répondre pour le cevoir la prochaine information
        String code = "";
        while (true){
          if (mySerial.available()>0){
            delay(CommunicationDelay);
            Serial.print("Decrypting cards details");
            while (mySerial.available() > 0) {
              char serialData = mySerial.read();
              code += String(serialData);
            }
            break;
          }
        }
        Serial.println(code);
        ModifyDataToDatabase(id, carte_id, solde, code);
        sendMsgToMaster("OK_CODE"); // réponse de fin validant que les données ont été modifiés
    }
  }
}


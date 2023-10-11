String carte_id;
String solde;
String carte_code;


// Variables Web
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

// Variables digicode
#include <Keypad.h>
const byte Ligne = 4; // Quatre lignes sur le digicode
const byte Colonne = 4; // Quatre colonnes sur le digicode
char hexaBouton[Ligne][Colonne] = { // On définit maintenant les symboles correspondant à chaque bouton
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}};
byte Ligne_Pins[Ligne] = {9, 8, 7, 6}; // On connecte la ligne du digicode 
byte Colonne_Pins[Colonne] = {5, 4, 3, 2}; // On connecte la colonne du digicode
Keypad mon_keypad = Keypad(makeKeymap(hexaBouton), Ligne_Pins, Colonne_Pins, Ligne, Colonne); // On initialise la librairie avec nos paramètres (la taille de la matrice et symboles)


// Variables RFID
#include <SPI.h> // SPI
#include <MFRC522.h> //RFID
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  //Code Web
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Connection au wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connecté ! Adresse IP : ");
  Serial.println(WiFi.localIP());

  //Code RFID
  SPI.begin();
  rfid.PCD_Init();

  //ECRAN de démarrage
  Serial.println("Arduino démarré!");
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



void SendDataToDatabase(int carte_id, int solde, int carte_code){
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

String GetCodeByDigicode() {
  const int nombre_de_caracteres = 4;
  String code = "";
  String a = "";

  while (code.length() < nombre_de_caracteres) {
    char caractere = mon_keypad.getKey();

    if (caractere) {
        if (caractere == 'A' || caractere == 'B' || caractere == 'C' || caractere == 'D') {
          Serial.println("Erreur : Code non valide");
          return "";
        }
        else if (caractere == '*') {
          Serial.println("Code annulé");
          code = "";
          a = "";
          continue;
        }

      code += caractere;
      delay(200); // Attendez un peu pour éviter de capturer plusieurs fois la même touche
      for (int i = 0; i <= code.length(); i++); {
      a += "#";
    }

    Serial.println(a);
    }
    
  }
  Serial.println("Code confidentiel saisi : " + code);
  return code;
}

String GetId() {
  String id = "";

  while (!rfid.PICC_IsNewCardPresent()) {
    //delay(50);
    //Serial.print("on attends");
    //return id;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      id += String(rfid.uid.uidByte[i], HEX);
    }

    Serial.print("Un badge est détecté. L'UID du tag est: ");
    Serial.println(id);

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    return id;
  }

  return id;
}


void loop() {
  //SendDataToDatabase(001, 100, 1234);

  //ECRAN scanner votre carte
  String RFID_id = "001"; //GetId();
  if (RFID_id != 0000 && RFID_id != "") {
    carte_id=RFID_id;
    Serial.println("Une carte à été reconnue!");
    //ECRAN entrer votre code confidentiel
    Serial.println("Entrer votre code confidentiel à 4 chiffres");
    String DigicodeCode = GetCodeByDigicode();
    data=ReceiveDataFromDatabase(carte_id);
    carte_code=ExtractFieldValue(data, "carte_code");
    solde=ExtractFieldValue(data, "solde");
    if (carte_code==DigicodeCode){
      //Ecran vous avez été correctement identifer
      Serial.println("carte_id: "+carte_id+"\nsolde: "+solde+"\n carte_code: "+carte_code);
    }
  }
}


/*
SendDataToDatabase(999999, 100, 5353);
delay(5000);
ReceiveDataFromDatabase(123456);
int solde=ExtractFieldValue(ReceiveDataFromDatabase(123456789), "solde");
Serial.println(solde);

String code = GetCodeByDigicode();

String carte_id = GetId();
if (carte_id != 0000 && carte_id != "") {
  Serial.println("Carte id reconnue!");
}
*/


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

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Connection au wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
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
  } else {
    Serial.println("Connection is not active, unable to ping.");
    delay(1000);
    return false;
  }
}



void SendDataToDatabase(String carte_id, String solde, String carte_code){
  if(IsConnectionActive()){
    Serial.println("Arduino>>>Server (active connection>>>execute function)");
    data = "carte_id=" + carte_id + "&solde=" + solde + "&carte_code=" + carte_code;

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



String ReceiveDataFromDatabase(int carte_id){
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

void loop() {
  
  SendDataToDatabase("009900", "00100000", "0330");
  //delay(5000);
  //ReceiveDataFromDatabase(123456);
  //int solde=ExtractFieldValue(ReceiveDataFromDatabase(123456789), "solde");
  //Serial.println(solde);
  //delay(20000);

}






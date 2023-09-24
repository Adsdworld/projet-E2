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
    unsigned long start = millis();
    if (client.println("PING / HTTP/1.1\r\nHost: www.google.com\r\n\r\n") > 0) {
      unsigned long pingTime = millis() - start;
      Serial.println("Ping successful. Response time (ms): " + String(pingTime));
    }
    client.stop();
    return true;
  } else {
    Serial.println("Ping failed.");
    delay(1000); // Wait before retrying
    return false;
  }
}

bool IsConnectionActive2(){
if (Ping.ping(host)) {
    //Serial.println("Ping réussi. La connexion Internet est active.");
    return true;
  } else {
    //Serial.println("Échec du ping. Pas de connexion Internet.");
    return false;
  }
}

void SendDataToDatabase(int carte_id, int solde, int carte_code){
  Serial.println("Arduino>>>Server (enter function)");
  if(IsConnectionActive()){
    Serial.println("Arduino>>>Server (execute function)");
    data = "carte_id=" + String(carte_id) + "&solde=" + String(solde) + "&carte_code=" + String(carte_code);

    String url = "http://" + server + "/add.php";
    HTTPClient http;
    http.begin(client, url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.POST(data);
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Data sent successfully.");
      // Handle the response from the server if needed
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
  }else{
    Serial.println("Connection is not active, unable to ping");
  }
  
  // Faire une vérification en gettant les données et vérifiant si elles match parfaitement.
}

void SendDataToDatabase2(int carte_id, int solde, int carte_code){
  Serial.println("Arduino>>>Server (enter function)");
  if(IsConnectionActive()){
    Serial.println("Arduino>>>Server (execute function)");
    data = "carte_id=" + String(carte_id) + "&solde=" + String(solde) + "&carte_code=" + String(carte_code);

    if (client.connect(server, 80)) {
      client.println("POST /add.php HTTP/1.1");
      client.println("Host: "+server); 
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(data.length());
      client.println();
      client.print(data);
    }

    if (client.connected()) {
      client.stop(); // Déconnexion du serveur
    }
  }else{
    Serial.println("Connection is not active, unable to ping");
  }
  
  // Faire une vérification en gettant les données et vérifiant si elles match parfaitement.
}

String ReceiveDataFromDatabase(int carte_id){
  Serial.println("Server>>>Arduino (enter function)");
  if(IsConnectionActive()){
    
    Serial.println("Server>>>Arduino (execute function)");
    String url = "http://"+server+"/get_data.php?carte_id=" + carte_id;
    HTTPClient http;
    http.begin(client, url);

    int httpCode = http.GET();
    String data_received;
    if (httpCode == HTTP_CODE_OK) {
      data_received = http.getString();
      Serial.println("Data received:");
      Serial.println(data_received);
      // Analysez les données JSON ici et affichez-les sur la console série
    } else {
      Serial.println("HTTP GET FAILED, check wireless connection");
      Serial.println(httpCode);
    }

    http.end();
    return data_received;
  }else{
    Serial.println("Connection is not active, unable to ping");
    return "null";
  }
}

int ExtractFieldValue(String dataReceived, String fieldName) {
  const size_t capacity = JSON_OBJECT_SIZE(4) + 60; //ici il y a 4 valeurs: id, carte_id, solde, carte_code >>> à ajuster
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
  
  SendDataToDatabase(888888, 1000000, 4949);
  //delay(5000);
  ReceiveDataFromDatabase(123456);
  //int solde=ExtractFieldValue(ReceiveDataFromDatabase(123456789), "solde");
  //Serial.println(solde);
  //delay(20000);

}






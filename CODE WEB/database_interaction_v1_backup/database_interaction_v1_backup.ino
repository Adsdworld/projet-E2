#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>



#include <SPI.h>

WiFiClient client;
#define WIFI_SSID "Redmi Note 8 Pro"
#define WIFI_PASS "asx135bko"
String data;
int carte_id = 123;
int solde = 100;
int carte_code = 0000;

void setup() {
  Serial.begin(115200);
  Serial.println("Loading...");
  delay(1000); // Donnez au capteur un peu de temps pour démarrer

  data = "";

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.print("Connecté ! Adresse IP : ");
  Serial.println(WiFi.localIP());
}

void loop() {
  data = "carte_id=" + String(carte_id) + "&solde=" + String(solde) + "&carte_code=" + String(carte_code);

  if (client.connect("projet-e2-eseo.000webhostapp.com", 80)) { // Remplacez par l'adresse de votre serveur
    client.println("POST /add.php HTTP/1.1");
    client.println("Host: projet-e2-eseo.000webhostapp.com"); // Adresse du serveur
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.print(data);
  }

  if (client.connected()) {
    client.stop(); // Déconnexion du serveur
  }

  delay(20000); // Attendez cinq minutes avant de renvoyer

  // Remplacez '123' par la carte_id souhaitée
  String carte_id = "123";

  // Créez l'URL avec la carte_id comme paramètre GET
  String url = "http://projet-e2-eseo.000webhostapp.com/get_data.php?carte_id=" + carte_id;

  HTTPClient http;
  http.begin(client, url);

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println("Data received:");
    Serial.println(payload);
    // Analysez les données JSON ici et affichez-les sur la console série
  } else {
    Serial.println("HTTP GET failed");
  }

  http.end();

  delay(20000); // Attendez avant la prochaine requête
}






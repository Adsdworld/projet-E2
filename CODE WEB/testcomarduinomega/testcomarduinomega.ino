#include <ESP8266WiFi.h>

const char* WIFI_SSID = "Redmi Note 8 Pro";
const char* WIFI_PASSWORD = "asx135bko";

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Connexion au réseau Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au WiFi en cours...");
  }

  Serial.print("Connecté au réseau WiFi. Adresse IP : ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Vous pouvez ajouter votre propre code ici pour d'autres fonctionnalités.
}

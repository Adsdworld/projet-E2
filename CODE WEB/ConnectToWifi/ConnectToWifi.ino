
//#include <ArduinoWiFiServer.h>
//#include <BearSSLHelpers.h>
//#include <CertStoreBearSSL.h>
//#include <ESP8266WiFiAP.h>
//#include <ESP8266WiFiGeneric.h>
//#include <ESP8266WiFiGratuitous.h>
//#include <ESP8266WiFiMulti.h>
//#include <ESP8266WiFiSTA.h>
//#include <ESP8266WiFiScan.h>
//#include <ESP8266WiFiType.h>

//#include <WiFiClientSecure.h>
//#include <WiFiClientSecureBearSSL.h>
//#include <WiFiServer.h>
//#include <WiFiServerSecure.h>
//#include <WiFiServerSecureBearSSL.h>
//#include <WiFiUdp.h>

// Librairies utilisées
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

WifiClient client;



// Set WiFi credentials
#define WIFI_SSID "Redmi Note 8 Pro"
#define WIFI_PASS "asx135bko"

void setup() {
  Serial.begin(115200);
  Serial.println();

}

void loop() {
  Serial.print("Connecting to ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

}
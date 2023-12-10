#include <SoftwareSerial.h>
#define rxPin D6
#define txPin D7
int CommunicationDelay=1000;
int ConsoleRefreshDelay=1000;
int SlaveTimeout=60000;
SoftwareSerial mySerial (rxPin, txPin);

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

void setup() {
  delay(5000);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(115200);
  Serial.begin(115200);
}

void loop() {
  Serial.println(readMsgFromSlave());
}


/*

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
  while(true){
        sendMsgToSlaveWithConfirmation("START");

    sendMsgToSlaveWithConfirmation("MENU");
    Serial.println("On est passé à la suite");
    String msg = readMsgFromSlaveWithTimeout();
    Serial.print("Retour du read with timeout: "+msg);
    if (msg==String("RFID")){ 
      break;
    }
    if (msg==String("MENUED")){ 
      break;
    }
  }
  delay(10000);
}

*/
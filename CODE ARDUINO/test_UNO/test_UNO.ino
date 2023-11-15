#include <SoftwareSerial.h>
#define rxPin 2
#define txPin 3
int CommunicationDelay=1000;
int ConsoleRefreshDelay=1000;
SoftwareSerial mySerial (rxPin, txPin);

void sendMsgToMaster(String message) {
  mySerial.print(message);
  Serial.println("Une message à été envoyé:"+message);
  delay(CommunicationDelay*4);
}
String readMsgFromMaster() {
  String receivedMessage = "";
  Serial.println("\n***Waiting for data available from Master");
  while (!mySerial.available()) {

  }
  Serial.print("New data available from Master");
  delay(CommunicationDelay); // Wait for the short message to arrive
  while (mySerial.available() > 0) {
    char serialData = mySerial.read();
    receivedMessage += String(serialData);
  }
  Serial.println("\n***Message reçu du Master:"+receivedMessage+"\n");
  return receivedMessage;
}

void setup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(115200);
}

void loop() {
  String receivedMessage = readMsgFromMaster();
  if (receivedMessage == "START") {
    sendMsgToMaster("OK");
    Serial.println("animation de l'écran");
    sendMsgToMaster("STARTED");
  }
}

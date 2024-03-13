//communication
#include <SoftwareSerial.h>
#define rxPin 15 // jour d'anniversaire de Martin & Arthur
#define txPin 14 // mois d'anniversaire de Arthur & Martin
int CommunicationDelay=1000;
//int ConsoleRefreshDelay=1000;
//int SlaveTimeout=60000;
SoftwareSerial mySerial (rxPin, txPin);

void COMSetup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);
}

void sendMsg(String message) {
  mySerial.print(message);
  Serial.println("Un message à été envoyé:"+message);
  delay(CommunicationDelay*3);
}
void sendMsgToSlaveWithConfirmation(String message) {
  Serial.println("\n***Sending '"+message+"' with confirmation to Slave");
  String receivedMessage = "";
  while (true){
    mySerial.print(message);
    //delay(CommunicationDelay*3);
    delay(10);
    if (mySerial.available()>0){
      receivedMessage = "";
      delay(CommunicationDelay); // Wait for the short message to arrive
      Serial.print("New data available from Slave");
      while (mySerial.available() > 0) {
        char serialData = mySerial.read();
        receivedMessage += String(serialData);
      }
      if (receivedMessage=="OK"){
        Serial.println("\n***Successfuly sended '"+message+"' with confirmation to Slave");
        return;
      } else {
        Serial.println("\nMessage reçu:"+receivedMessage);
      }
    }
    Serial.print(".");
  }
}

void setup() {
  COMSetup();
  Serial.begin(115200);
  Serial.println("La carte à démarré !");
}

void loop() {
  sendMsgToSlaveWithConfirmation("Message de test pour la communication");
}
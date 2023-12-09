#include <SoftwareSerial.h>
#define rxPin D6
#define txPin D7
int CommunicationDelay=1000;
int ConsoleRefreshDelay=1000;
int SlaveTimeout=60000;
SoftwareSerial mySerial (rxPin, txPin);

void sendMsgToSlave(String message) {
  mySerial.print(message);
  Serial.println("Une message à été envoyé:"+message);
}
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
void sendMsgToSlaveWithConfirmation(String message) {
  Serial.println("\n***Sending '"+message+"' with confirmation to Slave");
  String receivedMessage = "";
  while (true){
    mySerial.print(message);
    delay(CommunicationDelay*3);
    if (mySerial.available()>0){
      receivedMessage = "";
      delay(CommunicationDelay/4); // Wait for the short message to arrive
      Serial.print("New data available from Slave");
      while (mySerial.available() > 0) {
        char serialData = mySerial.read();
        receivedMessage += String(serialData);
      }
      if (receivedMessage=="OK"){
        Serial.println("\n***Successfuly sended '"+message+"' with confirmation to Slave");
        return;
      }
    }
  }
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
        if (receivedMessage=="KA" || receivedMessage=="KAKA"){
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
}
void loop() {
  while(true){
    sendMsgToSlaveWithConfirmation("MENU");//affiche une animation cool de démarrage
    String msg = readMsgFromSlaveWithTimeout();
    Serial.print("Retour du read with timeout: "+msg);
    if (msg==String("MENUED")){ //Attends que l'arduino est finit son animation peut, permet le KeepAlive
      break;
    }
  }
  delay(10000);
}

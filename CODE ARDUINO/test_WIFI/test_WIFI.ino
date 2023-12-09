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
    delay(ConsoleRefreshDelay);
  }
  Serial.println("\nNew data available from Slave");
  delay(CommunicationDelay); // Wait for the short message to arrive
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
      delay(CommunicationDelay); // Wait for the short message to arrive
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

void setup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(115200);
}
void loop() {
  sendMsgToSlaveWithConfirmation("START");
  delay(10000);
}

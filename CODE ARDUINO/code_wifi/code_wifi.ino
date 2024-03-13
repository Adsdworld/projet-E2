/****************************************************************************************************************************************
* Main:                                                                                                                                 *
*   Contient les Variables essentiels.                                                                                                  *
****************************************************************************************************************************************/
int TimeoutMsgResponse=1000; //Temps d'attente pour recevoir un message
int TimeoutArduinoUno=60000; //Temps d'attente pour comprendre que la UNO est KO
String ArduinoUnoData;       //Message reçu de UNO, peut être positif ou indéchiffrable, sert de stockage tampon pour vérifier si le message est reconnu
String carte_id;
String carte_code;
String solde; //débat String ou int?
bool DebugSerialPrinln=false;

/****************************************************************************************************************************************
* UART COMMUNICATION:                                                                                                                   *
*   Contient les fonctions de communications                                                                                            *
****************************************************************************************************************************************/
#include <SoftwareSerial.h>
#define rxPin D14
#define txPin D15 // j'ai essayé d'inverser ok ici (pas check l'envoie mais reception ok)
int CommunicationDelay=1000;
int ConsoleRefreshDelay=1000;
int SlaveTimeout=60000;
SoftwareSerial mySerial (rxPin, txPin);

void sendMsgToMaster(String message) {
  mySerial.print(message);
  Serial.println("Un message à été envoyé:"+message);
  delay(CommunicationDelay*4);
}

String readMsgFromSlaveWithTimeout() {
    int Timeout=millis()+SlaveTimeout;
    String receivedMessage = "";
    Serial.println("\n***Waiting for data available from Slave with Timeout:"+String(SlaveTimeout)+"ms");
    while (millis()<Timeout){
      if (mySerial.available()>0){
        Serial.print("New data available from Slave");
        delay(CommunicationDelay); // Wait for the short message to arrive
        while (mySerial.available() > 0) {
          char serialData = mySerial.read();
          receivedMessage += String(serialData);
        }
        Serial.println("\n***Message reçu du Slave:"+receivedMessage+"\n");
        if (receivedMessage=="KA"){
          Timeout=millis()+SlaveTimeout;
        }
        else{
          return receivedMessage;
        }
      }
  }
  return "KO";
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
      }
    }
  }
}

void COMSetup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(115200);
}

void setup() {
  COMSetup();
  Serial.println();
  Serial.println("La carte à démarré !");
  //sendMsgToSlaveWithConfirmation("Message de test pour la communication");
}


void loop() {
  if (0<mySerial.available()){
    //mySerial.flush();
    String StringBuilder="";
    delay(TimeoutMsgResponse); //laisser un peu de temps pour que le court message arrive
    while(mySerial.available()>0){
      char Serialdata =mySerial.read();
      StringBuilder+=String(Serialdata);
    }
    Serial.println(StringBuilder);
    if (StringBuilder == "Message de test pour la communication"){ //Message de test pour la communication
      sendMsgToMaster("OK");
    }
  }
}





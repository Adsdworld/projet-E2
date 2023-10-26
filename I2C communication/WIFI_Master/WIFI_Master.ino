int TimeoutMsgResponse=1000;
int TimeoutArduinoUno=60000;
String ArduinoUnoData;
String carte_id;


void Shiny(int nb, int ms){
  for (int i=1;i<nb;i++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(ms);
    digitalWrite(LED_BUILTIN, LOW);
    delay(ms);
  }
  return;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

String ReadMsgFromArduino(){
  int Timeout=millis()+TimeoutArduinoUno;
  String StringBuilder="";
  while(millis()<Timeout){
    if (0<Serial.available()){
      delay(TimeoutMsgResponse); //laisser un peu de temps pour que le court message arrive
      while(Serial.available()>0){
        char Serialdata =Serial.read();
        StringBuilder+=String(Serialdata);
      }
      if (0==Serial.available()){
        return StringBuilder;
      }
    }
  }
  return "KO";
}

void SendMsgToArduino(String Message){
  while(true) {
    Serial.flush();
    Serial.print(Message);
    delay(TimeoutMsgResponse);
    if (0<Serial.available()){
      String StringBuilder="";
      while(Serial.available()>0){
        char Serialdata =Serial.read();
        StringBuilder+=String(Serialdata);
      }
      if (StringBuilder==String("OK")){
        break;
      }
    }
  }
  return;
}

void loop() {
  while(true){ // structure de code pour envoyer un ordre et récupérer des données après l'ordre ici l'id de la carte
    SendMsgToArduino("1");
    ArduinoUnoData=ReadMsgFromArduino();
    if (ArduinoUnoData!="KO"){ //Si on reçoit KO, on renvoie refait la boucle jusqu'a ce que l'arduino uno ais redémarrer/ fait son travail, ça signifie aussi que l'arduino uno doit intégrer un timeout < TimeoutArduinoUno
      carte_id=ArduinoUnoData;
      break;}}

  Serial.println("Carte détecté: "+carte_id);
  Shiny(20, 50);
}

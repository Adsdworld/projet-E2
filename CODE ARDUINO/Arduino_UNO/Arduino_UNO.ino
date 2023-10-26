int TimeoutMsgResponse=1000;
int TimeoutArduinoUno=20000;

String ReadInstantMsgFromArduino(){
  String StringBuilder="";
  while(true){
    if (0<Serial.available()){
      delay(TimeoutMsgResponse); //laisser un peu de temps pour que le court message arrive
      while(Serial.available()>0){
        char Serialdata =Serial.read();
        StringBuilder+=String(Serialdata);
      }
      return StringBuilder;
    }
  }
  return "NO MESSAGE FOUND";
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (0<Serial.available()){
    String StringBuilder="";
    delay(TimeoutMsgResponse); //laisser un peu de temps pour que le court message arrive
    while(Serial.available()>0){
      char Serialdata =Serial.read();
      StringBuilder+=String(Serialdata);
    }
    if (StringBuilder==String("START")){//affiche une animation cool de démarrage
      Serial.print("OK");
      Start();
      Serial.print("STARTED");
    }
    else if(StringBuilder==String("CONNECTING")){
      Serial.print("OK");
      Connecting();
    }
    else if (StringBuilder==String("WELCOME")){//affiche une animation cool de démarrage
      Serial.print("OK");
      Welcome();
      Serial.print("TOUCH");
    }
    else if(StringBuilder==String("RFID")){
      Serial.print("OK");
      RFID();
    }
    else if(StringBuilder.charAt(0)=='D'){
      Serial.print("OK");
      DigiCode(StringBuilder);
    }
    else if(StringBuilder==String("DATABASE")){
      Serial.print("OK");
      DataBase();
    }
    else{
      Serial.print("ERROR");
    }
  }
}

void Start(){
  int Timeout=millis()+TimeoutArduinoUno;
  while(true){
    while(millis()<Timeout){
      //Some screen animations, do not exceed TimeoutArduinoUno or découper en petite animations et vérifier le timetout entre chaques pour svoir si oui ou non envoyer un KeepAlive
      return;
    }
    Serial.print("KA");
  }
}
void Connecting(){
  while(true){
    //3 fast wifi connecting screen animation
    if (ReadInstantMsgFromArduino()=="CONNECTED"){
      return;
    }
  }
}
void Welcome(){
  int Timeout=millis()+TimeoutArduinoUno;
  while(true){
    while(millis()<Timeout){
      //if (screen is touch){return;}
    }
    Serial.print("KA");
  }
}
void RFID(){
  while(true){
    //animations courtes?
    if (ReadInstantMsgFromArduino()=="RFIDED"){
      return;
    }
  }
}
void DigiCode(String Message){
  if (Message=="D0"){
    //_ _ _ _
  }
  else if (Message=="D1"){
   //* _ _ _   
  }
  else if (Message=="D2"){
    // * * _ _
  }
  else if (Message=="D3"){
    // * * * _
  }
  else if (Message=="D4"){
    // * * * * use '#' to validate code
  }
}
void DataBase(){
  while(true){
    //animations courtes? rond qui tourne
    if (ReadInstantMsgFromArduino()=="DATABASED"){
      return;
    }
  }
}
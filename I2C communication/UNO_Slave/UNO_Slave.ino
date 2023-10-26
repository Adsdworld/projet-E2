int TimeoutMsgResponse=1000;

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

void loop() {
  if (0<Serial.available()){
    String StringBuilder="";
    delay(TimeoutMsgResponse); //laisser un peu de temps pour que le court message arrive
    while(Serial.available()>0){
      char Serialdata =Serial.read();
      StringBuilder+=String(Serialdata);
    }
    if (StringBuilder==String("SHINY")){
      Serial.print("OK");
      Serial.print(GetCardID());
    }
    else if(StringBuilder==String("SHINO")){
      Serial.print("OK");
      Shiny(10, 500);
    }
    else{
      Serial.print("ERROR");
    }
  }
}


String GetCardID(){
  delay(1000);
  Shiny(100, 50);
  return "D2HJ82";
}
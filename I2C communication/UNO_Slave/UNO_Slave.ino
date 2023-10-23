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
      delay(TimeoutMsgResponse); //laisser un peu de temps pour que le court message arrive
      while(Serial.available()>0){
        char Serialdata =Serial.read();
        StringBuilder+=String(Serialdata);
      }
      if (0==Serial.available()){
        if (Serialdata=='1'){
          Serial.print("OK");
          Serial.print(GetCardID());
        }
      }
    }
  /*if (0!=Serial.available()){ //Serial.isavailable() renvoie la quantité d'octets disponible 
    char Serialdata =Serial.read();
    if (Serialdata=='1'){
      Serial.print("OK");
      Serial.print(GetCardID());
    }
  }*/ //ancien code le nouveau en théorie permet de récupérer plus que 1 caractère
}


String GetCardID(){
  delay(10000);
  Shiny(100, 50);
  return "D2HJ82";
}
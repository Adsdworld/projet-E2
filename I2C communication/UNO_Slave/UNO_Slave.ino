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
  delay(10000);
  Shiny(10, 100);
}

void loop() {
  if (0!=Serial.available()){ //Serial.isavailable() renvoie la quantité d'octets disponible 
    char Serialdata =Serial.read();
    if (Serialdata=='1'){
      Serial.print("OK");
      Serial.print(GetCardID());
    }
  }
}


String GetCardID(){
  delay(10000);
  Shiny(100, 50);
  return "D2HJ82";
}
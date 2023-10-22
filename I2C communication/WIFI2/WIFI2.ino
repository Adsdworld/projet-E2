//#include <SoftwareSerial.h>

//SoftwareSerial mySerial(2, 3);  // Crée un port série logiciel sur les broches 2 (RX) et 3 (TX)

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);  // Démarre la communication série à 9600 baud
}

void loop() {
  if (Serial.available()) {
    char data = Serial.read();  // Lire un caractère série entrant
    if (data=='B'){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      data='!';
    }
  }
  digitalWrite(LED_BUILTIN, LOW);
  delay(5000);
}

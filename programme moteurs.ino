nbbillets = 10;
capteur = 0;

void setup() {
  // put your setup code here, to run once:
  for (int i = 3; i < 7; i++){
    pinMode(i, OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < nbbillets; i++) {
    while (capteur != 1)  {
      moteurssens1();
    }
    moteuroff();
    delay(0.2);
    while capteur == 1) {
      motorssens2();
      delay(0.2);
    }
    moteuroff();
    delay(0.2);
  }
}

void moteurssens1() {
  digitalWrite(3, 1);
  digitalWrite(4, 1);
  digitalWrite(5, 0);
  digitalWrite(6, 1);
}
void moteurssens2() {
  digitalWrite(3, 1);
  digitalWrite(4, 0);
  digitalWrite(5, 1);
  digitalWrite(6, 1);
}
void moteursoff() {
  for (int i = 3; i <7 ; i++){
    digitalWrite(i, 0);
  }
}
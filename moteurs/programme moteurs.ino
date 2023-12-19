#include <Stepper.h>
const int nbpas = 200;
Stepper pasapas(nbpas, 10, 11, 12, 13);

int nbbillets = 10;
int capteur = 0;

void setup() {
  // put your setup code here, to run once:
  for (int i = 3; i < 13; i++){
    pinMode(i, OUTPUT);
  }
  pinMode(2, INPUT);
  pasapas.setSpeed(80);
}

void loop() {
  // put your main code here, to run repeatedly:
  int x = 1; // 1 = distribue, 2 = reçoit
  if (x == 1){
    moteurpp(x);
    for (int i = 0; i < nbbillets; i++) {
      //faire tourner moteur pas à pas dans le sens de distrib sur le billet de 10
      while (capteur != 1)  { // tant que le capteur de détecte pas de billet, tourner pour envoyer un billet
        moteurssens1();
        capteur = digitalRead(2);
      }
      moteursoff();
      delay(0.5);
      while (capteur == 1) {  //quand le capteur détecte un billet, tourner pour finir d'envoyer le billet et tourner autre moteur dans autre sens pour renvoyer le deuxième billet eventuel
        moteurssens2();
        capteur = digitalRead(2);
      }
      delay(0.2);
      moteursoff();
      delay(0.2);
    }
  }
  if (x == 2){
    // mettre une led au rouge pour dire qu'on peut pas rentrer de billet 
    moteurpp(x);
    for (int i = 0; i < nbbillets; i++) {
      // mettre une led au vert pour dire qu'on peut rentrer un billet
      while (capteur != 1)  { // tant que le capteur de détecte pas de billet, tourner pour envoyer un billet
        moteurssens2(); 
      }
      // mettre la led au rouge pour dire qu'il faut attendre pour remettre un billet
      delay(0.2);
      moteursoff();
      delay(0.5);
      // détecter la couleur du billet pour la suite
      while (capteur == 1) {  //quand le capteur détecte un billet, tourner pour finir d'envoyer le billet et tourner autre moteur dans autre sens pour renvoyer le deuxième billet eventuel
        moteurssens2();
      }
      moteursoff();
      delay(0.2);
    }
  }
}

void moteurpp(int sens){
  // faire faire 1 tour au moteur dans le bon sens
  if (sens == 1)  {
    pasapas.step(nbpas);
  }
  else {
    pasapas.step(-nbpas);
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
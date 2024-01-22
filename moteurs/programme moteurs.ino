// Le programme a été fait en considérant le capteur encore non implémenté, les lignes de code l'incluants sont donc mises en commentaire

#include <Stepper.h>
const int nbpas = 200;
Stepper pasapas(nbpas, 8, 9, 10, 11);
int ledV = 12;
int ledR = 13;

int nbbillets = 10;
int capteur = 0;

void setup() {

  for (int i = 2; i <= 13; i++){
    pinMode(i, OUTPUT);
  }
  pinMode(A2, INPUT);
  pasapas.setSpeed(80); // set up la vitesse du moteur pas à pas
  digitalWrite(4, 0); // arrêter les moteurs
  digitalWrite(5, 0);
}

void loop(/*int x, int billets*/) { // x est la variable qui déterminera le sens de rotation (déterminé via l'écran) x == 1 --> retrait, x == 2 --> dépôt, la variable billet sera aussi déterminée via l'écran
  int x = 1; // défini arbitrairement pour ne pas créer d'erreur dans la compilation (peut arbitrairement être modifié à 2)
  if (x == 1){ //tourne pour distribuer les billets à l'utilisateur
    moteurpp(x, nbpas);
    for (int i = 0; i < nbbillets; i++) {
      //while (capteur != 1)  { // tant que le capteur de détecte pas de billet, tourner pour envoyer un billet (pas encore implémenté)
        moteur1sens1();
        moteur2sens1();
        delay(1000);
      //  capteur = digitalRead(2);
      //}
      moteursoff();
      delay(0.5);
      //while (capteur == 1) {  //quand le capteur détecte un billet, tourner pour finir d'envoyer le billet et tourner autre moteur dans autre sens pour renvoyer le deuxième billet eventuel
        moteur1sens2();
        moteur2sens1();
        delay(1000);
      //  capteur = digitalRead(2);
      //}
      moteursoff();
      delay(0.1);
    }
  }
  if (x == 2){ //tourne pour recevoir les billets de l'utilisateur
    moteurpp(x, nbpas);
    for (int i = 0; i < nbbillets; i++) {
      //while (capteur != 1)  { // tant que le capteur de détecte pas de billet, tourner pour envoyer un billet
        moteur1sens2();
        delay(2000); // ici le capteur n'est pas implémenté il faut donc tourner pensant 2s pour qu'un billet rentre
      //  digitalRead(capteur);
      //}
      moteursoff();
      /*delay(0.5);
      while (capteur == 1) {  //quand le capteur détecte un billet, tourner pour finir d'envoyer le billet
        moteur1sens2();
        delay(1000);
        digitalRead(capteur)  
      }
      moteursoff();
      delay(0.2);*/
    }
  }
}

void moteurpp(int sens, int nbpas){
  // faire faire 1 tour au moteur dans le bon sens
  if (sens == 1)  {
    pasapas.step(nbpas);
  }
  else {
    pasapas.step(-nbpas);
  }
}

void moteur1sens1() {
  digitalWrite(4, 1);
  digitalWrite(2, 1);
  digitalWrite(3, 0);
}
void moteur1sens2() {
  digitalWrite(4, 1);
  digitalWrite(2, 0);
  digitalWrite(3, 1);
}
void moteur2sens1() {
  digitalWrite(5, 1);
  digitalWrite(6, 1);
  digitalWrite(7, 0);
}
void moteur2sens2() {
  digitalWrite(5, 1);
  digitalWrite(6, 0);
  digitalWrite(7, 1);
}
void moteursoff() {
  for (int i = 2; i <= 7 ; i++){
    digitalWrite(i, 0);
  }
}
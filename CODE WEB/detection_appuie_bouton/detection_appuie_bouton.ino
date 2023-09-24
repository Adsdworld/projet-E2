/*commentaire :
Description : Permet de récupérer 0 ou 1 à partir d'un bouton connecté à du 5v
Etat : Fonctionnel
Schéma : Disponible

Attention à bien respecté le schéma de branchement du bouton, certaines branches du bouton sont connectés entre elles.
*/


//déclaration des variables
int gpio_input=D8;                             //on se sert de la 9 pour savoir si le courant passe où non
int gpio_output=D2;   //sert d'allimentation

void setup() {
  pinMode(gpio_input, INPUT);                 //entrée numérique
  pinMode(gpio_output, OUTPUT);
  Serial.begin(9600);                         //ceci dit à l'arduino d'intéragir avec la console à une vitesse de 9600 bauds, on peut ouvrir la console avec la loupe en haut à droite sur arduino, la console apparaitra vide si vôtre arduino n'y à rien écris, plus loin on va écrire des choses sur la console
  digitalWrite(gpio_output, HIGH);
}


void loop() {

  Serial.print(digitalRead(gpio_input));             //arduino affiche sur la console "etat du bouton"


}
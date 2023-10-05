const int Ligne = 4;
const int Colonne = 4;

char hexaBouton[Ligne][Colonne] = {
  {'1', '2', '3', 'a'},
  {'4', '5', '6', 'b'},
  {'7', '8', '9', 'c'},
  {'*', '0', '#', 'd'}
};

int lignesPins[Ligne] = {13}; // Broches pour les lignes (D0 à D3)
int colonnesPins[Colonne] = {D4, 4, 5, 16}; // Broches pour les colonnes (D4 à D7)//D3 ne s"allume pas en output

void setup() {
  // Initialiser les broches de colonne en sortie avec HIGH
  for (int i = 0; i < Colonne; i++) {
    pinMode(colonnesPins[i], OUTPUT);
    digitalWrite(colonnesPins[i], HIGH);
  }

  // Initialiser les broches de ligne avec une résistance de pull-up interne
  for (int i = 0; i < Ligne; i++) {
    pinMode(lignesPins[i], INPUT_PULLUP);
  }
  Serial.begin(115200);
  Serial.println("début");
}

String Digicode(){
  String code="";
  String a="";
  while (code.length() < Ligne) {
    //Serial.println("1 check effectué");
    for (int i = 0; i < Colonne; i++) {
      // Activer la colonne en cours
      digitalWrite(colonnesPins[i], LOW);
      
      // Lire les lignes
      for (int j = 0; j < Ligne; j++) {
        if (digitalRead(lignesPins[j]) == LOW) {
          // Un bouton a été pressé dans la ligne j et la colonne i
          char boutonPresse = hexaBouton[j][i];
          //Serial.println(boutonPresse);
          code+=boutonPresse;
          a="";
          for (int k = 1; k <= code.length(); k++) {
            a += "#";
          }
          Serial.println(a);
          // Attendez que le bouton soit relâché
          while (digitalRead(lignesPins[j]) == LOW) {
            delay(10);
          }
        }
      }

      // Désactiver la colonne en cours
      digitalWrite(colonnesPins[i], HIGH);
    }
    
  }
  return code;
}

void loop() {
  String code=Digicode();
  Serial.print(code);
}

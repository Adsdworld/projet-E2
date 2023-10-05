const int Ligne = 4;
const int Colonne = 4;

char hexaBouton[Ligne][Colonne] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

int lignesPins[Ligne] = {16, 14, 4, 0}; // Broches pour les lignes (D0 à D3)
int colonnesPins[Colonne] = {D0, D1, D2, D3}; // Broches pour les colonnes (D4 à D7)

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

void loop() {
  Serial.println("1 check effectué");
  for (int i = 0; i < Colonne; i++) {
    // Activer la colonne en cours
    digitalWrite(colonnesPins[i], LOW);
    
    // Lire les lignes
    for (int j = 0; j < Ligne; j++) {
      if (digitalRead(lignesPins[j]) == LOW) {
        // Un bouton a été pressé dans la ligne j et la colonne i
        char boutonPresse = hexaBouton[j][i];
        Serial.print(String(boutonPresse)+" "+String(colonnesPins[i])+" "+String(lignesPins[j]));
        
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
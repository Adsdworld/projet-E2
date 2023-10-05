#include <Keypad.h>

const byte Ligne = 4; // Quatre lignes sur le digicode
const byte Colonne = 4; // Quatre colonnes sur le digicode

// On définit maintenant les symboles correspondant à chaque bouton
char hexaBouton[Ligne][Colonne] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte Ligne_Pins[Ligne] = {D7, D6, D5, D4}; // On connecte la ligne du digicode 
byte Colonne_Pins[Colonne] = {D3, D2, D1, D0}; // On connecte la colonne du digicode

//byte Ligne_Pins[Ligne] = {D0, D1, D2, D3}; // On connecte la ligne du digicode 
//byte Colonne_Pins[Colonne] = {D4, D5, D6, D7}; // On connecte la colonne du digicode

// On initialise la librairie avec nos paramètres (la taille de la matrice et symboles)
Keypad mon_keypad = Keypad(makeKeymap(hexaBouton), Ligne_Pins, Colonne_Pins, Ligne, Colonne); 

String GetCodeByDigicode() {
  const int nombre_de_caracteres = 4;
  String code = "";
  String a = "";

  while (code.length() < nombre_de_caracteres) {
    char caractere = mon_keypad.getKey();

    if (caractere) {
        if (caractere == 'A' || caractere == 'B' || caractere == 'C' || caractere == 'D') {
          Serial.println("Erreur : Code non valide");
          return "";
        }
        else if (caractere == '*') {
          Serial.println("Code annulé");
          code = "";
          a = "";
          continue;
        }

      code += caractere;
      delay(200); // Attendez un peu pour éviter de capturer plusieurs fois la même touche
      for (int i = 0; i <= code.length(); i++); {
      a += "#";
    }

    Serial.println(a);
    
    }
    
  }
  Serial.println("Code confidentiel saisi : " + code);
  return code;
}

void setup() {
  Serial.begin(115200);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
}

void loop() {
  String code = GetCodeByDigicode();
  //tester sur la database
}
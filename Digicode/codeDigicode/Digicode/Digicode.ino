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

byte Ligne_Pins[Ligne] = {9, 8, 7, 6}; // On connecte la ligne du digicode 
byte Colonne_Pins[Colonne] = {5, 4, 3, 2}; // On connecte la colonne du digicode

// On initialise la librairie avec nos paramètres (la taille de la matrice et symboles)
Keypad mon_keypad = Keypad(makeKeymap(hexaBouton), Ligne_Pins, Colonne_Pins, Ligne, Colonne); 

String GetCodeByDigicode() {
  const int nombre_de_caracteres = 4;
  String code = "";
  String a = "";

  while (code.length() < nombre_de_caracteres) {
    char caractere = mon_keypad.getKey();
 
    if (caractere) {
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
  Serial.begin(9600);
}

void loop() {
  String code = GetCodeByDigicode();
  //testee-r sur la database
}
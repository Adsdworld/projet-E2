//char Mymessage[10]; //Initialized variable to store recieved data
//char res[10]="0123456789"; //Initialized variable to store recieved data
String serial_read="";
String te="/*Wifi#001$*/";

  void setup() {
    Serial.begin(9600);
    Serial.println("Started");
  }
  
//code: #001
//cartecible %wifi
//pouvoir séparer les infos

  String FindCaracter(String chaine,String caracter){
    bool find=false;
    String findstring="";
    for (int i=0;i<chaine.length();i++){
      if (find==false){
        Serial.println(chaine[i]);
        if (chaine[i]==caracter[0]){
          Serial.println("caractere trouvé");
          find=true;
          findstring+=chaine[i];
        }
      }else{
        Serial.println("ajout des caractères:");
        Serial.println(chaine[i]);
        findstring+=chaine[i];
      }
    }
    Serial.println("fin de la fonction-->return");
    return findstring;
  }


  void loop() {
    //serial_read=Serial.read();
    //Serial.println(FindCaracter(te, "#"));
    Serial.println(FindCaracter(te, "#"));
    delay(30000);
  }
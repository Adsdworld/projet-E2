bool IsConnectionActive2(){
if (Ping.ping(host)) {
    //Serial.println("Ping réussi. La connexion Internet est active.");
    return true;
  } else {
    //Serial.println("Échec du ping. Pas de connexion Internet.");
    return false;
  }
}

bool IsConnectionActive(){
  if (client.connect(host, 80)) {
    //unsigned long start = millis();
    if (client.println("PING / HTTP/1.1\r\nHost: www.google.com\r\n\r\n") > 0) {
      //unsigned long pingTime = millis() - start;
      //Serial.println("Ping successful. Response time (ms): " + String(pingTime));
    }
    client.stop();
    return true;
  } else {
    Serial.println("Connection is not active, unable to ping.");
    delay(1000);
    return false;
  }
}*


void SendDataToDatabase2(int carte_id, int solde, int carte_code){
  if(IsConnectionActive()){
    Serial.println("Arduino>>>Server (active connection>>>execute function)");
    data = "carte_id=" + String(carte_id) + "&solde=" + String(solde) + "&carte_code=" + String(carte_code);

    if (client.connect(server, 80)) {
      client.println("POST /add.php HTTP/1.1");
      client.println("Host: "+server); 
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(data.length());
      client.println();
      client.print(data);
    }
    if (client.connected()) {
      client.stop(); // Déconnexion du serveur
    }
  }else{
    Serial.println("Connection is not active, unable to ping");
  }
  
  // Faire une vérification en gettant les données et vérifiant si elles match parfaitement.
}
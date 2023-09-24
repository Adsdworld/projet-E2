## Informations
- server = "projet-e2-eseo.000webhostapp.com"; localhost est utilisable sur le site web
- user = "id21292157_projete2user";
- pass = "projet-E2-password";
- database = "id21292157_projete2database";

## Requêtes à la Database 
#Passer le carte_id en CHAR pour régler le soucis des 0 non comptabiliser avant
CREATE TABLE information_table (
    id INT AUTO_INCREMENT PRIMARY KEY,
    carte_id INT, 
    solde INT,
    carte_code CHAR(4)
);



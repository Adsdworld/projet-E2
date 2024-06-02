## Requêtes à la Database 
### Version 1
#Passer le carte_id en CHAR pour régler le soucis des 0 non comptabiliser avant
CREATE TABLE information_table (
    id INT AUTO_INCREMENT PRIMARY KEY,
    carte_id INT, 
    solde INT,
    carte_code CHAR(4)
);

### Version 2
CREATE TABLE information_table (
    A INT AUTO_INCREMENT PRIMARY KEY,
    I VARCHAR(10), 
    S VARCHAR(12),
    C CHAR(4),
    P VARCHAR(12)
);

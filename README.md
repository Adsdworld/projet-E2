# Projet-E2

![Projet Icon](/IMAGES/image.ico)

## Auteurs
- [Carla Joulin](https://github.com/CarlaJoulin)
- [Arthur de Sallier Dupin](https://github.com/ArthurDupin)
- [Lilian](https://github.com/LilianGitHub)
- [Martin](https://github.com/MartinGitHub)
- [Ghyslain](https://github.com/GhyslainGitHub)
- [Nathan](https://github.com/NathanGitHub)

## Projet-E2

### Description
Réalisation d'un distributeur à billets comprenant un lecteur RFID, un écran TFT, un digicode et une puce WiFi intégrer sur une arduino.

### Matériel
- une carte arduino wifi D1 R1 (wemos) intégrant un module ESP8266
- une carte arduino uno (méga 2560)
- un écran tactile (240*320px)
- un digicode (key pad arduino 4*4)
- un lecteur RFID (#RC522)
- deux badges RFID
- une demi-breadboard et des cables

## Installation hardware
- Le branchement du Digicode est effectué sur des broches se suivant pour facilité l'initialisation de broches.
- Installer la communication RX TX
- Avant d'installer le lecteur RFID, Installer l'écran en respectant le montage indiqué, le lecteur RFID utilisera les mêmes branchements SPI que l'écran à savoir CLOCK, MOSI, MISO

## Installation du site
- 000 webhost gratuit ou Apache
- Dans code web uploader les fichiers
- créer une base de données et mettre à jour les informations
- schéma disponible dans MYSQL/query.md

## Licence
MIT.

---
Projet de deuxième année à l'ESEO sur Angers année 2023/2024


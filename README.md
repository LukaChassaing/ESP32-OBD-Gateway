# Passerelle OBD-II CAN basée sur ESP32

Ce projet est une passerelle bus CAN OBD-II basée sur ESP32 avec connectivité WiFi pour le diagnostic et la surveillance des véhicules. Elle permet de lire et d'interpréter les données du bus CAN d'un véhicule via le port OBD-II, et de rendre ces données accessibles via WiFi grâce à un serveur web.

## Fonctionnalités

- Lit les données du véhicule depuis le port OBD-II via le bus CAN
- Supporte les PIDs OBD-II standard (Mode 01)
- Récupère des paramètres tels que :
  - Régime moteur
  - Vitesse du véhicule
  - Température d'admission d'air
  - Pression d'huile moteur
  - Pression de suralimentation (si disponible)
- Fournit un serveur web pour accéder aux données via WiFi
- Facile à configurer et à étendre

## Matériel

- Carte microcontrôleur ESP32
- Transceiver CAN (par exemple, MCP2551)
- Câble OBD-II vers DB9
- Optionnel : PCB personnalisé pour un design plus compact et robuste

## Logiciel

- Arduino IDE
- Noyau Arduino ESP32
- Bibliothèque Arduino CAN

## Démarrage

1. Clonez ce dépôt
2. Ouvrez le projet dans l'IDE Arduino
3. Installez les bibliothèques nécessaires (noyau Arduino ESP32, Arduino CAN)
4. Configurez les paramètres WiFi dans le code (SSID et mot de passe)
5. Connectez les composants matériels :
   - ESP32 CAN RX au transceiver CAN RX
   - ESP32 CAN TX au transceiver CAN TX
   - Transceiver CAN CANH et CANL au câble OBD-II
   - Alimentez l'ESP32 depuis le port OBD-II ou une source externe
6. Flashez le code sur l'ESP32
7. Connectez-vous au réseau WiFi de l'ESP32
8. Ouvrez un navigateur web et accédez à l'adresse IP de l'ESP32

## Utilisation

Une fois connecté au serveur web, vous pouvez accéder aux données du véhicule via les endpoints suivants :

- `/` : Message de bienvenue
- `/rpm` : Régime moteur
- `/speed` : Vitesse du véhicule
- `/iat` : Température d'admission d'air
- `/oilpressure` : Pression d'huile moteur
- `/boostpressure` : Pression de suralimentation (si disponible)

Les données sont renvoyées sous forme de valeurs en texte brut.

## Contribution

Les contributions sont les bienvenues ! N'hésitez pas à soumettre une Pull Request.

## Licence

Ce projet est open-source et disponible sous la [Licence MIT](LICENSE).

## Remerciements

- [Arduino](https://www.arduino.cc/)
- [Noyau Arduino ESP32](https://github.com/espressif/arduino-esp32)
- [Bibliothèque Arduino CAN](https://github.com/sandeepmistry/arduino-CAN)

## Contact

Pour toute question ou suggestion, veuillez ouvrir une issue sur GitHub.

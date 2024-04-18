#include <Arduino.h>
#include <CAN.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Pins ESP32
#define CAN_RX 4
#define CAN_TX 5

// Paramètres WiFi
const char* ssid = "MonReseau";
const char* password = "MotDePasse";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  
  // Initialisation CAN
  CAN.setPins(CAN_RX, CAN_TX);
  if (!CAN.begin(500E3)) {
    Serial.println("Erreur initialisation CAN");
    while (1);
  }
  
  // Connexion WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au WiFi...");
  }
  
  Serial.println("Connexion WiFi établie");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());
  
  // Routes serveur web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Bienvenue sur le serveur OBD!");
  });
  
  server.on("/rpm", HTTP_GET, [](AsyncWebServerRequest *request){
    int rpm = getEngineRPM();
    request->send(200, "text/plain", String(rpm));
  });
  
  server.on("/speed", HTTP_GET, [](AsyncWebServerRequest *request){
    int speed = getVehicleSpeed();
    request->send(200, "text/plain", String(speed));
  });
  
  server.on("/iat", HTTP_GET, [](AsyncWebServerRequest *request){
    int iat = getIntakeAirTemp();
    request->send(200, "text/plain", String(iat));
  });
  
  server.on("/oilpressure", HTTP_GET, [](AsyncWebServerRequest *request){
    int oilPressure = getOilPressure();
    request->send(200, "text/plain", String(oilPressure));
  });
  
  server.on("/boostpressure", HTTP_GET, [](AsyncWebServerRequest *request){
    int boostPressure = getBoostPressure();
    request->send(200, "text/plain", String(boostPressure));
  });
  
  server.begin();
}

void loop() {
  // Traiter les requêtes CAN en continu
  if (CAN.parsePacket()) {
    // Filtrer et traiter les trames OBD ici 
    // ...
  }
}

// Récupérer la température d'admission d'air via OBD
int getIntakeAirTemp() {
  CAN.beginPacket(0x7DF, 8);
  CAN.write(0x02);
  CAN.write(0x01); 
  CAN.write(0x0F); // PID pour IAT 
  CAN.endPacket();

  unsigned long timeout = millis() + 100;
  while (millis() < timeout) {
    if (CAN.parsePacket()) {
      if (CAN.read() == 0x41 && CAN.read() == 0x0F) {
        return CAN.read() - 40; // Valeur décalée de 40°C
      }
    }
  }

  return 0;  
}

// Récupérer la pression d'huile moteur via OBD
int getOilPressure() {
  CAN.beginPacket(0x7DF, 8);
  CAN.write(0x02);
  CAN.write(0x01);
  CAN.write(0x5C); // PID pour Oil Pressure  
  CAN.endPacket();
  
  unsigned long timeout = millis() + 100;
  while (millis() < timeout) {
    if (CAN.parsePacket()) {
      if (CAN.read() == 0x41 && CAN.read() == 0x5C) {
        return CAN.read(); // Pression en kPa
      }
    }
  } 
  
  return 0;
}

// Récupérer la pression de suralimentation via OBD
int getBoostPressure() {
  CAN.beginPacket(0x7DF, 8);
  CAN.write(0x02);
  CAN.write(0x01);
  CAN.write(0x60); // PID pour Boost Pressure
  CAN.endPacket();

  unsigned long timeout = millis() + 100;
  while (millis() < timeout) {
    if (CAN.parsePacket()) {
      if (CAN.read() == 0x41 && CAN.read() == 0x60) {
        return CAN.read(); // Pression en kPa
      }
    }
  }
  
  return -1; // -1 si non supporté  
}

// Récupérer le régime moteur via OBD
int getEngineRPM() {
  CAN.beginPacket(0x7DF, 8);
  CAN.write(0x02); // Nombre d'octets de données 
  CAN.write(0x01); // Mode 
  CAN.write(0x0C); // PID
  CAN.endPacket();

  unsigned long timeout = millis() + 100;
  while (millis() < timeout) {
    if (CAN.parsePacket()) {
      if (CAN.read() == 0x41 && CAN.read() == 0x0C) {
        int high = CAN.read();
        int low = CAN.read();
        return ((high << 8) | low) / 4;
      }
    }
  }

  return 0; // Échec
}

// Récupérer vitesse véhicule via OBD
int getVehicleSpeed() {
  CAN.beginPacket(0x7DF, 8);
  CAN.write(0x02);
  CAN.write(0x01); 
  CAN.write(0x0D); 
  CAN.endPacket();

  unsigned long timeout = millis() + 100;
  while (millis() < timeout) {
    if (CAN.parsePacket()) {
      if (CAN.read() == 0x41 && CAN.read() == 0x0D) {
        return CAN.read();
      }
    }
  }

  return 0;  
}

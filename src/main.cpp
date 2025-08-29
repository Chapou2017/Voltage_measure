//Si tu veux améliorer la précision :
//
//    Ajoute un condensateur (ex. 100nF) entre la broche analogique et la masse pour stabiliser la lecture.
//
//    Utilise analogReadResolution(12); dans setup() pour t’assurer que la résolution est bien à 12 bits.

#include <Arduino.h>

// Définition des broches 
const int analogPin = 33; // Choisis une broche analogique stable

// Définition des variables
const float R1 = 30000.0; // 30kΩ
const float R2 = 7500.0;  // 7.5kΩ
float tension_mesure = 0.00;
int rawValue = 0;
float tension_moteur = 0.00;

float corrigerTension(float tension_lue) {
  return 1.016 * tension_lue + 0.71;
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  rawValue = analogRead(analogPin);
  tension_mesure = (rawValue / 4095.0) * 3.3; // Conversion en tension réelle
  tension_moteur = tension_mesure * ((R1 + R2) / R2);    // Recalcul de la tension d’entrée
  tension_moteur = corrigerTension(tension_moteur);

  Serial.print("Tension pin33: ");
  Serial.print(tension_mesure);
  Serial.println(" V");
    Serial.print("Tension mesurée : ");
  Serial.print(tension_moteur);
  Serial.println(" V");
  Serial.println();

  delay(500);
}

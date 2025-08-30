//Si tu veux améliorer la précision :
//
//    Ajoute un condensateur (ex. 100nF) entre la broche analogique et la masse pour stabiliser la lecture.
//
//    Utilise analogReadResolution(12); dans setup() pour t’assurer que la résolution est bien à 12 bits.

#include <Arduino.h>
#include <TM1637Display.h>

// Broches pour le TM1637
#define CLK 25
#define DIO 26

TM1637Display display(CLK, DIO);

// Broche analogique
const int analogPin = 33;

// Résistances du pont diviseur
const float R1 = 30000.0;
const float R2 = 7500.0;

// Variables
float tension_mesure = 0.00;
int rawValue = 0;
float tension_moteur = 0.00;

float corrigerTension(float tension_lue) {
  return 1.016 * tension_lue + 0.72;
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  display.setBrightness(2); // Luminosité max (0 à 7)
   
}

void loop() {
  
  rawValue = analogRead(analogPin);
  tension_mesure = (rawValue / 4095.0) * 3.3;
  tension_moteur = tension_mesure * ((R1 + R2) / R2);
  tension_moteur = corrigerTension(tension_moteur);

  Serial.print("Tension pin33: ");
  Serial.print(tension_mesure);
  Serial.println(" V");

  Serial.print("Tension mesurée : ");
  Serial.print(tension_moteur);
  Serial.println(" V");
  Serial.println();

  // Affichage sur TM1637 (ex: 1.23V → affiche 123)
  int valeurAffichee = (int)(tension_moteur * 100); // 2 chiffre après la virgule
  
  Serial.print("Valeur affichée : ");
  Serial.print(valeurAffichee);
  Serial.println(" V");
  Serial.println();

  if (tension_moteur > 11.5) {
    // Clignotement : alterne affichage / extinction
    display.clear();
    delay(250);
    display.showNumberDecEx(valeurAffichee, 0b01000000, false); // Affiche avec point décimal entre digit 2 et 3
    delay(250);
  } else {
    // Affichage normal
    display.showNumberDecEx(valeurAffichee, 0b01000000, false); // Affiche avec point décimal entre digit 2 et 3
    delay(500);
  }
  
}

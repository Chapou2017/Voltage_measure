#include <Arduino.h>

const int analogPin = 32; // Choisis une broche analogique stable
const float R1 = 10000.0; // 10kΩ
const float R2 = 4700.0;  // 4.7kΩ

void setup() {
  Serial.begin(115200);
}

void loop() {
  int rawValue = analogRead(analogPin);
  float voltage = (rawValue / 4095.0) * 3.3; // Conversion en tension réelle
  float Vin = voltage * ((R1 + R2) / R2);    // Recalcul de la tension d’entrée

  Serial.print("Tension mesurée : ");
  Serial.print(Vin);
  Serial.println(" V");

  delay(500);
}

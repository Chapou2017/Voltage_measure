#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <TM1637Display.h>

// Broches pour le TM1637
//#define CLK 25
//#define DIO 26

//TM1637Display display(CLK, DIO);

// Broche analogique
const int analogPinV1 = 33;
const int analogPinV2 = 32;
const int analogPinV3 = 35;

// Résistances du pont diviseur
const float R1 = 30000.0;
const float R2 = 7500.0;

// Variables de mesure de tension
//float tension_mesure_1 = 0.00;
//float tension_mesure_2 = 0.00;
int rawValue_1 = 0;
int rawValue_2 = 0;
int rawValue_3 = 0;
float tension_moteur_1 = 0.00;
float tension_moteur_2 = 0.00;
float tension_alimentation = 0.00;

// Écran 1 : I2C matériel sur GPIO 21 (SDA) et 22 (SCL)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled1(U8G2_R0, U8X8_PIN_NONE);

// Écran 2 : I2C logiciel sur GPIO 4 (SDA) et 16 (SCL)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C oled2(U8G2_R0, 16, 4, U8X8_PIN_NONE);

// Écran 3 : I2C logiciel sur GPIO 17 (SDA) et 5 (SCL)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C oled3(U8G2_R0, 5, 17, U8X8_PIN_NONE);

// Variables pour clignotement si dépassement seuil tension
bool clignote1 = true;
bool clignote2 = true;
bool clignote3 = true;

// fonction de correction de mesure de tension
float corrigerTension(float tension_lue) {
  return 1.016 * tension_lue + 0.72;
}

// Fonction de calcul de tension
float MesureTension(int voltage) {
  float tension_mesure = (voltage / 4095.0) * 3.3;
  float tension_reelle = tension_mesure * ((R1 + R2) / R2);
  tension_reelle = corrigerTension(tension_reelle);
  return tension_reelle;
}

// Fonction d'affichage avec alerte
void afficherTension(U8G2 &ecran, float tension, const char* titre, bool &clignote) {
  ecran.clearBuffer();

  ecran.setFont(u8g2_font_unifont_t_symbols);
  ecran.drawStr(0, 10, titre);

  if (tension > 11.5) {
    ecran.setFont(u8g2_font_unifont_t_symbols);
    ecran.drawStr(100, 10, "/!\\");
    clignote = !clignote;
  } else {
    clignote = true;
  }

  if (clignote) {
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%.2f v", tension);
    ecran.setFont(u8g2_font_logisoso30_tr);
    ecran.drawStr(0, 55, buffer);
  }

  ecran.sendBuffer();
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  //display.setBrightness(2); // TM1637 - Luminosité max (0 à 7)
  Wire.begin(21, 22); // Bus matériel pour oled1
  oled1.begin();
  oled2.begin();      // SW I2C ne dépend pas de Wire pour oled2
  oled3.begin();      // SW I2C ne dépend pas de Wire pour oled3
}

void loop() {
  
  rawValue_1 = analogRead(analogPinV1);
  rawValue_2 = analogRead(analogPinV2);
  rawValue_3 = analogRead(analogPinV3);
  tension_moteur_1 = MesureTension(rawValue_1);
  tension_moteur_2 = MesureTension(rawValue_2);
  tension_alimentation = MesureTension(rawValue_3);

  //Serial.print("Tension pin33: ");
  //Serial.print(tension_mesure);
  //Serial.println(" V");

  Serial.print("Tension mesurée M1 : ");
  Serial.print(tension_moteur_1);
  Serial.println(" V");
  Serial.println();
  Serial.print("Tension mesurée M2 : ");
  Serial.print(tension_moteur_2);
  Serial.println(" V");
  Serial.println();
  Serial.println("------------------------");

  afficherTension(oled1, tension_moteur_1, "Moteur 1", clignote1);
  afficherTension(oled2, tension_moteur_2, "Moteur 2", clignote2);
  afficherTension(oled3, tension_alimentation, "Tension alim", clignote3);

  // Affichage sur TM1637 (ex: 1.23V → affiche 123)
  //int valeurAffichee = (int)(tension_moteur * 100); // 2 chiffre après la virgule
  
  //Serial.print("Valeur affichée : ");
  //Serial.print(valeurAffichee);
  //Serial.println(" V");
  //Serial.println();

  //if (tension_moteur > 11.5) {
  //  // Clignotement : alterne affichage / extinction
  //  display.clear();
  //  delay(250);
  //  display.showNumberDecEx(valeurAffichee, 0b01000000, false); // Affiche avec point décimal entre digit 2 et 3
  //  delay(250);
  //} else {
  //  // Affichage normal
  //  display.showNumberDecEx(valeurAffichee, 0b01000000, false); // Affiche avec point décimal entre digit 2 et 3
  //  delay(500);
  //}
  delay(500);
}

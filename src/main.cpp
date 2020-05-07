#include <Arduino.h>

// Test variabler, skal komme fra andre steder i programmet
float HR = 120;

/* 
   Loader en string, og opstiller float variabler,
   hvor de enkelte dele af string StrData bliver læst ind.
   for at læse delene ind, skal der refreres til positionen i 
   string, og trækkes 48 fra, for at få det realle tal. Efter
   skal der ganges med 10 eller 100, afhængigt af positionen.
   Til slut lægges det hele sammen. Højde skal bruges som
   meter, men er indtastet i cm, derfor divideres der med 100.
*/

String StrData = "9324190145"; // Skal komme fra andet sted
float kg     = (StrData[0] - 48) * 10 + (StrData[1] - 48);
float age    = (StrData[2] - 48) * 10 + (StrData[3] - 48);
float height = ((StrData[4] - 48) * 100 + (StrData[5] - 48) * 10 + (StrData[6])) / 100;
float gender = (StrData[7] - 48);
float HRrest = (StrData[8] - 48) * 10 + (StrData[9] - 48);

float HRmax = 208 - (age * 0.7);    // Udregn HRmax
float HRreserve = HRmax - HRrest;   // Udregn HRreserve
unsigned long startTime = millis(); // Gemmer start tiden

void setup() {
  Serial.begin(115200);

  int StrLength = StrData.length(); // Tjekker at brugerdata er udfyldt
  if (StrLength == !10) {
    Serial.println("Mangler brugerdata");
  }
}

void loop() {
  float VO2rest;

  if (HR < 25) {    // Tjekker at EKG-sensoren sender en realistisk HR
    Serial.println("Tjek EKG-sensor");
    delay(5000);
  }

  unsigned long endTime = millis(); // Gemmer slut tiden
  unsigned long excerciseTime = (endTime - startTime) / 60000; // Udregner motionstid i minutter
  float VO2max = 3.542 + (-0.014 * age) + (0.015 * kg) + (-0.011 * HRrest); // Udregner VO2max

  if (gender == 2) { // Udregner VO2rest for kvinde eller mand
    VO2rest = (((655.1 + (9.56 * kg) + (1.85 * height) - (4.68 * age)) / 24) / 60) / 5;
  }
  else {
    VO2rest = (((66.5 + (13.75 * kg) + (5.03 * height) - (6.75 * age)) / 24) / 60) / 5;
  }

  float VO2reserve = VO2max - VO2rest;        // Udregner VO2reserve
  float VO2pulse = VO2reserve / HRreserve;    // Udregner VO2pulse
  float THR = HR - HRrest;                    // Udregner THR
  float EVO2 = THR * VO2pulse + VO2rest;      // Udregner EVO2
  float EE = EVO2 * 5 * excerciseTime;        // Udregner EE
  Serial.print("EE: "); Serial.println(EE);
  delay(2000);
}
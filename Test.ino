#include <Wire.h>                           // Folosita pentru comunicarea prin protocolul I2C
#include <MS5611.h>                         // Folosita pentru a interactiona cu senzorul de presiune si temperatura MS5411 / GY63

MS5611 ms5611;
double presiuneSol;

void debug_ms5611(double t, long p, float a, float A);

void setup() {
  Serial.begin(9600);
  Serial.println("Initializare senzor MS5611 / GY63");
  while(!ms5611){
    Serial.println("Problema la conectarea senzorului MS5611 / GY63");
    delay(500);
  }
  presiuneSol = ms5611.readPressure();                                        // Presiunea initiala (de la sol) este folosita pentru a calcula altitudinea
}

void loop() {
  double temperatura = ms5611.readTemperature();
  long presiune = ms5611.readPressure();
  float altitudineAbsoluta = ms5611.getAltitude(presiune);                    // Altitudine masurata de la nivelul marii
  float altitudine = ms5611.getAltitude(presiune, presiuneSol);               // Altitudinea reala
  
  //debug_ms5611(temperatura, presiune, altitudineAbsoluta, altitudine);
  //delay(500);
}

void debug_ms5611(double t, long p, float a, float A){
  Serial.println("-------------------");
  Serial.print("Temperatura: ");
  Serial.println(t);
  Serial.print("Presiune: ");
  Serial.println(p);
  Serial.print("Altitudinea relativa marii: ")
  Serial.println(a);
  Serial.print("Altitudinea reala: ");
  Serial.println(A);
  Serial.println();
}

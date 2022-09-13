#include <Wire.h>                           // Folosita pentru comunicarea prin protocolul I2C
#include <MS5611.h>                         // Folosita pentru a interactiona cu senzorul de presiune si temperatura MS5411 / GY63
#include <MQ135.h>                          // Folosita pentru a interactiona cu senzorul de gaze MQ135
#include <SD.h>
#include <SPI.h>

File myFile;
int pinCS = 8;                              // D8 
MS5611 ms5611;
MQ135 mq135_sensor(A0);                     // Senzorul de gaze necesita calibrare si un mic setup code ce il vom adauga la momentul constructiei
double presiuneSol;

void debug_ms5611(double t, double p, double a, double A);

void setup() {
  Serial.begin(9600);
  pinMode(pinCS, OUTPUT);
  
  if(SD.begin()){
    Serial.println("Card SD OK");
  }
  else{
    Serial.println("Problema la conectarea cardului SD");
  }
  
  Serial.println("Initializare senzor MS5611 / GY63");
  while(!ms5611.begin()){
    Serial.println("Problema la conectarea senzorului MS5611 / GY63");
    delay(500);
  }
  presiuneSol = ms5611.getPressure();                                        // Presiunea initiala (de la sol) este folosita pentru a calcula altitudinea

  myFile = SD.open("date.csv", FILE_WRITE);                                   // Initializare cap de tabel 
  if(myFile){
    myFile.println("Altitudine,Presiune,Temperatura,PPM");
    myFile.close();
  }
  else{
    Serial.println("Eroare la deschiderea fisierului date.csv");
  }
}

void loop() {
  double temperatura = ms5611.getTemperature();                                               // Masurata in grade celsius
  double presiune = ms5611.getPressure();                                                     // Masurata in milibari 1 mBa = 100 Pa
  double altitudine = 44330.0 * (1-pow(presiune/presiuneSol, 1/5.255));                       // Altitudinea rachetei
  float ppm = mq135_sensor.getPPM();                                                          // Calitate aer

  myFile = SD.open("date.csv", FILE_WRITE);
  if(myFile){
    myFile.print(altitudine);
    myFile.print(",");
    myFile.print(presiune);
    myFile.print(",");
    myFile.print(temperatura);
    myFile.print(",");
    myFile.println(ppm);
    myFile.close();
  }
  else{
    Serial.println("Eroare la deschiderea fisierului date.csv");
  }
  
  //debug_ms5611(temperatura, presiune, altitudineAbsoluta, altitudine);
  //delay(500);
}

void debug_ms5611(double t, double p, double a, double A){
  Serial.println("-------------------");
  Serial.print("Temperatura: ");
  Serial.println(t);
  Serial.print("Presiune: ");
  Serial.println(p);
  Serial.print("Altitudinea relativa marii: ");
  Serial.println(a);
  Serial.print("Altitudinea reala: ");
  Serial.println(A);
  Serial.println();
}

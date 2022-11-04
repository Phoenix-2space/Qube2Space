#include <SPI.h>
#include <SD.h>
#include "MS5611.h"

MS5611 MS5611(0x77);

#ifndef LED_BUILTIN
#define LED_BUILTIN    13
#endif

const int chipSelect = 8;

uint32_t start, stop;

int smokeA0 = A0;

double altitudine_sol = 0.0;

File dataFile;

double func(double current_pressure, double sea_level_pressure=1013.25){
    double a = current_pressure/sea_level_pressure;
    double b = 1/5.255;
    double exp = 44330 * (1 - pow(a, b));
    return exp;
}

double altitudine(double presiune_senzor){
    return func(presiune_senzor) - altitudine_sol;
}

void setup() {
  // serial
  Serial.begin(9600);
  while (!Serial);
  // pinmode
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(smokeA0, INPUT);
  // sd begin
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  // sd end
  // ms begin
   if (MS5611.begin() == true)
  {
    Serial.println("MS5611 found.");
  }
  else
  {
    Serial.println("MS5611 not found. halt.");
    while (1)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
  }
  int result = MS5611.read();
  altitudine_sol = func(MS5611.getPressure());
  Serial.println(altitudine_sol);
  // ms end
  Serial.println();
  if(SD.exists("datalog.csv")) {
    SD.remove("datalog.csv");
  }
  dataFile = SD.open("datalog.csv", FILE_WRITE);
}

void sensor_values()
{
  start = micros();
  int result = MS5611.read();
  stop = micros();
  int analogSensor = analogRead(smokeA0);
  if (result != MS5611_READ_OK)
  {
    Serial.print("Error in read: ");
    Serial.println(result);
  }
  else
  {
    Serial.print("T:\t");
    Serial.print(MS5611.getTemperature(), 2);
    dataFile.print(MS5611.getTemperature(), 2);
    dataFile.print(", ");
    Serial.print("\tP:\t");
    Serial.print(altitudine(MS5611.getPressure()));
    dataFile.print(altitudine(MS5611.getPressure()));
    dataFile.print(", ");
    Serial.print("\tt:\t");
    Serial.print(stop - start);
    dataFile.print(stop - start);
    Serial.print("S:\t");
    Serial.print(analogSensor);
    dataFile.print(", ");
    dataFile.print(analogSensor);
    dataFile.println();
    Serial.println();
  }
  dataFile.flush();
}


void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  MS5611.setOversampling(OSR_STANDARD);
  sensor_values();
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
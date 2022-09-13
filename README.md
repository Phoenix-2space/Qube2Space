# Qube2Space
Codul este in mare parte completat. Mai raman de testat valorile senzorilor si eventual modificata modalitatea de obtinere a altitudinii. Senzorul de gaze necesita
calibrare si putin cod boilerplate pe care il vom adauga la testare.

Librarii standard:
#include <Wire.h>
#include <SD.h>
#include <SPI.h>

Librarii obtinute din Library Manager-ul Arduino:
#include <MQ135.h>
#include <MS5611.h>

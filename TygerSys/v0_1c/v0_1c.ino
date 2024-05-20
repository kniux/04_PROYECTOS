/*
  Proyecto: Tygersys version para pantalla LCD 16x2
  ver: 0.1b
  Rev: 13/05/2024 
  Autor: Edgar Cano
    
  Descripción: 
  El sistema contienen:
  - 3 botones:
      - Con B1, el sistema entra a modo ajuste de la fecha y hora del RTC.
      - Con B3, el sistema entra a modo ajuste de las alarmas del RTC.
      - Con B2:
              - Se muestra la visualizan como están configuradas las alarmas
              - En modo ajuste, se incrementan los parametros correspondientes.
  - 1 buzzer para indicar la activacion de alguna alarma    
  - 1 RTC DS 3231, el cual contiene un sensor de temperatura con parte entera (MSB) y parte flotante (LSB)
*/

#include "TygerSys.h"
 
void setupManaged();
void operationModeManaged(int mode);
void controlTyger();
void displayRTCManaged();

void setup() {
  setupManaged(); 
}
void loop() {
  controlTyger();
}


/*
 * Name: Weather Station 
 * Date: 09/06/2022
 * Author: Edgar Cano
 * Ver: 1.1
 * Desc:  
 *        - Estación meteorologica, proporciona 
 *          humedad, temperatura, hora y fecha...
 * HW:    
 *        - Sensor DS3231/DHT11/Pantalla LCD.
 * Revisión:
 *        - Limpieza de código
 */
#include <DHT.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DHTPin  4
#define DHTType DHT11

/* Definiendo FSM del sistema */
#define S_HOME            0
#define S_REFRESH_SCREEN  1
#define S_READ_SENSORS    2
#define S_SWITCH_SCREEN   3 
uint8_t systemState = 0;    

/* Definiendo FSM para pantalla activa*/
#define SCREEN_WELCOME    0
#define SCREEN_DATEHOUR   1
#define SCREEN_HUMTEMP    2
uint8_t screenState = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2); 
DHT dht11(DHTPin, DHTType);
RTC_DS3231 rtc;
 
unsigned long previousMillisState1 = 0;
long Timeinterval_1 = 500;     // Refresh sccreen

unsigned long previousMillisState2 = 0;
long Timeinterval_2 = 2000;     // Read Sensor

unsigned long previousMillisState3 = 0;
long Timeinterval_3 = 5000;     // Switch Screen

int tempDHT = 0;
int Hum = 0;
float tempDS = 0;

void Control_System_Management();
void Screen_Management();
void Sensor_Management();


// Subsistema de Control Principal *************************************
void setup() {
  //Serial.begin(9600);
  dht11.begin();
  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  lcd.init();
  lcd.backlight();                  
  delay(500);  
}
void loop(){
  FSM_System_Transitions();  
  FSM_System();  
}

// End of code

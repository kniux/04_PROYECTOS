// Librerías
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
#include <Wire.h>
#include <Servo.h>
#include <TM1637Display.h>    // incluye libreria TM1637

// Definiciones y constantes

#define NULLSTATE       0
#define SET_DATETIME    1
#define DISPLAY_ALARMS  2
#define SET_ALARMS      3
#define OFF_ALARMS      4
#define FOOD_DELIVERY   5
uint8_t systemsate =  NULLSTATE;

const int button1   =  5;                   
const int button2   =  4;                   
const int button3   =  3;                   
const int button4   =  2;                   
const int buzzer_pin =  6;                   
const int LED_pin =  10;                   
const int CLK = 52;
const int DIO = 53;

// Variables declaration
bool alarm1_status, alarm2_status;

char Time[]     = "  :  :   ",
     calendar[] = "        /  /20  ",
     alarm1[]   = "A1:   :  :00 ",
     alarm2[]   = "A2:   :  :00 ",
  temperature[] = "T:    C";

byte  i, second, minute, hour, day, date, month, year,
      alarm1_minute, alarm1_hour, alarm2_minute, alarm2_hour,
      status_reg;

uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
uint8_t done[] = {
		 SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,      // d
		 SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
		 SEG_C | SEG_E | SEG_G,              // n
		 SEG_A | SEG_D | SEG_E | SEG_F | SEG_G      // E
		};


int pos = 0;    // Variable para almacenar la posición del servo.      

// Objetos
LiquidCrystal_I2C lcd(0x27, 16, 2);        
Servo myservo; 
TM1637Display display(CLK, DIO);  // crea objeto




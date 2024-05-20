#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
#include <Wire.h>

#define NULLSTATE       0
#define SET_DATETIME    1
#define DISPLAY_ALARMS  2
#define SET_ALARMS      3
#define OFF_ALARMS      4
uint8_t systemsate =  NULLSTATE;

LiquidCrystal_I2C lcd(0x27, 16, 2);  
 
const int button1   =  5;                   
const int button2   =  4;                   
const int button3   =  3;                   
const int buzzer_pin =  6;                   
const int LED_pin =  10;                   
 
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
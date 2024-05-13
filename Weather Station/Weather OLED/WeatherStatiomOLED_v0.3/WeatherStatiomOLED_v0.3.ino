/*
 * ---------------------------------------------------------------------------
 * Name: Estación meteorologica OLED 
 * Author: Edgar Cano
 * Date:   25/02/2022
 * Ver:    0.3
 * Board:  Arduino Pro mini 5V, 16 MHz
 * ---------------------------------------------------------------------------
 * DESCRIPCION:     
 *      - Uso del componente generico 280 (no Adafruit)
 *      - Adaptación de librerías genericas #include "i2c.h" e "i2c_BMP280.h"
 *      - Hay lectura de humedad con DTH.h by Adafruit.
 *      - Se elimina parpadeo de pantalla por marcos
 *      - Se elimina de la FSM Pantalla de bienvenida.
 * ---------------------------------------------------------------------------
 * ERRORES:     
 *      - NO JALA dia de la semana con daysOfTheWeek[now.dayOfTheWeek()]);  
 * ---------------------------------------------------------------------------      
 * MEJORAS A AÑADIR:      
 *      - Ninguna      
 */

#include <Wire.h>                        // Include Wire library (required for I2C devices)
#include <Adafruit_GFX.h>                // Include Adafruit graphics library
#include <Adafruit_SSD1306.h>            // Include Adafruit SSD1306 OLED driver
#include <RTClib.h>
#include <DHT.h>
#include "i2c.h"
#include "i2c_BMP280.h"

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_RESET 1

#define oledLoading     0
#define oledDateTime    1
#define oledTemHum      2
#define oledPressAlt    3
uint8_t state= oledDateTime;

#define DHTPin  9
#define DHTType DHT11

Adafruit_SSD1306 display(128, 64);  // declara la resolucion del display
BMP280 bmp280;
RTC_DS3231 rtc;
DHT dht11(DHTPin, DHTType);

float temperature;
float pascal;
int hum;
byte second, minute, hour, day, date, month, year;
static float meters, metersold;

long intervalOledRefresh = 500;
unsigned long previousMillisOledRefresh = 0;

long intervalBme = 2000;
unsigned long previousMillisBme = 0;

long intervalOledScreen = 5000;
unsigned long previousMillisOledScreen = 0;

void setup(void) {  
  Serial.begin(57600);
  dht11.begin();

  delay(1000); 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // inicia la comunicacion I2C con el display que tiene la direccion 0x3C  
  display.setRotation(0);  
  display.clearDisplay();
  display.display(); 
  display.setTextColor(WHITE, BLACK);    
  
  Serial.print("Probe BMP280: ");
  if (bmp280.initialize()) Serial.println("Sensor found");
  else{
      Serial.println("Sensor missing");
      while (1) {}
  }
  // onetime-measure:
  bmp280.setEnabled(0);
  bmp280.triggerMeasurement();   

  rtc.begin();
 // rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
}
void loop(){
/*   Máquina de Estados Finitos   */
  unsigned long currentMillis = millis();

  if((currentMillis - previousMillisOledRefresh ) >= (intervalOledRefresh)){
      previousMillisOledRefresh = currentMillis;
      oledInfo();
  }
  if((currentMillis - previousMillisBme ) >= (intervalBme)){
      previousMillisBme = currentMillis;
      readDataBme280();
  }
  if((currentMillis - previousMillisOledScreen ) >= (intervalOledScreen)){
      previousMillisOledScreen = currentMillis;
      OledChangeScreen();
  }  
}

void oledInfo(){
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);  

  switch( state){
    case oledLoading:
        displayLoading();
        break;
    case oledDateTime:
        displayDateTime();
        break;
    case oledTemHum:
        displayTempHum();
        break;    
    case oledPressAlt:
        displayPressAlt();
        break;  
    default:
    break;                
  } 
}

void readDataBme280(){
    bmp280.awaitMeasurement();
    bmp280.getTemperature(temperature);
    bmp280.getPressure(pascal);    
    bmp280.getAltitude(meters);
    metersold = (metersold * 10 + meters)/11;
    bmp280.triggerMeasurement();
    hum = dht11.readHumidity();
}

void OledChangeScreen(){
  switch(state){
    case oledDateTime:
      state= oledTemHum;
    break; 
    case oledTemHum:
      state= oledPressAlt;
    break; 
    case oledPressAlt:
      state= oledDateTime;
    break;   
    default:
    break;  
  }
}

void displayLoading(){
  /*
    DateTime now = rtc.now();
    display.setTextSize(2);
    display.setCursor(17, 20);
    if( now.dayOfTheWeek()==1) display.print("LUNES");
    if( now.dayOfTheWeek()==2) display.print("MARTES");   
    if( now.dayOfTheWeek()==3) display.print("MIERCOLES");
    if( now.dayOfTheWeek()==4) display.print("JUEVES");   
    if( now.dayOfTheWeek()==5) display.print("VIERNES");  
    if( now.dayOfTheWeek()==6) display.print("SABADO");   
    if( now.dayOfTheWeek()==7) display.print("DOMINGO");  
    display.display();
  */  
}

void displayDateTime(){  
  DateTime now = rtc.now();
  char bufferDate[15] = "00/00/0000";
  formatoFecha(bufferDate, now.day(), now.month(), now.year());
  oledPrintChar(2,5,17, bufferDate);
  
  char bufferTime[11] = "00:00:00";
  formatoHora(bufferTime, now.hour(), now.minute(), now.second());
  oledPrintChar(2,17,36, bufferTime);    
  oledPrintChar(1,20,4, "WEATHER STATION");
  
  display.drawRect(0,0,128,64, WHITE);
  display.display();
}

void displayTempHum(){
  oledPrintChar(1,30, 3, "TEMPERATURA");
  oledPrintFloat(2,25, 15, temperature);
  display.print(" C");
  oledPrintChar(1,42, 35, "HUMEDAD");
  oledPrintFloat(2,25, 46, hum);
  display.print(" %");
  display.fillCircle(92,17,2, WHITE);
  display.drawRoundRect(0,0,128,64,15,WHITE);
  display.drawLine(0,32,128,32,WHITE);
  display.display();
}

void displayPressAlt(){
  oledPrintChar(1,42, 3, "PRESION");
  oledPrintFloat(2,4, 15, pascal);
  display.setCursor(74, 15);
  display.print(" hPa");
  oledPrintChar(1,42, 35, "ALTITUD");
  oledPrintFloat(2,25, 46, metersold);
  display.print(" m");
  display.drawRoundRect(0,0,128,64,15,WHITE);
  display.drawLine(0,32,128,32,WHITE);
  display.display();
}

void oledPrintChar(int text_size, int x_pos, int y_pos, char text[]){
  display.setTextSize(text_size);
  display.setCursor(x_pos, y_pos);
  display.print(text);
//  display.display();
}
void oledPrintFloat(int text_size, int x_pos, int y_pos, float data){
  display.setTextSize(text_size);
  display.setCursor(x_pos, y_pos);
  display.print(data);
 // display.display();
}

void formatoFecha(char bufferFecha[12], int numDia, int numMes, int numA){
  sprintf(bufferFecha, "%02d/%02d/%04d",numDia,numMes,numA);
}
void formatoHora(char bufferHora[12], int hora, int minutos, int segundos){
  sprintf(bufferHora, "%02d:%02d:%02d",hora,minutos,segundos);
}

// End of Code

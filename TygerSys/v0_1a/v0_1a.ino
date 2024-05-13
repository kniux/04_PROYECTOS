/*
  Proyecto: Tygersys version para pantalla LCD 20x4
  ver: 0.1a
  Autor: Edgar Cano
  Rev: 13/05/2024   
*/

#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  
 
const int button1   =  5;                   
const int button2   =  4;                   
const int button3   =  3;                   
const int buzzer_pin =  6;                   
const int LED_pin =  10;                   
 
// Variables declaration
bool alarm1_status, alarm2_status;
char Time[]     = "  :  :  ",
     calendar[] = "      /  /20  ",
     alarm1[]   = "A1:   :  :00", alarm2[]   = "A2:   :  :00",
     temperature[] = "T:   .   C";
byte  i, second, minute, hour, day, date, month, year,
      alarm1_minute, alarm1_hour, alarm2_minute, alarm2_hour,
      status_reg;
 
void Alarm() {
  if (alarm1_status || alarm2_status){
    /*VERIFICAR SI EL TIEMPO ALARMA COINCIDE CON EL RELOJ*/
    if( second == 0 && hour == alarm1_hour && minute == alarm1_minute ){
       digitalWrite(buzzer_pin, HIGH);
       digitalWrite(LED_pin, HIGH);
    }  
    if( second == 0 && hour == alarm2_hour && minute == alarm2_minute ){
       digitalWrite(buzzer_pin, HIGH);
       digitalWrite(LED_pin, HIGH);
    } 
  }
}
void DS3231_read(){                             // Función para leer datos de tiempo y calendario.
  Wire.beginTransmission(0x68);                 // Inicia el protocolo I2C con la dirección del DS3231.
  Wire.write(0);                                // Enviar la dirección del registro.
  Wire.endTransmission(false);                  // Reinicio de I2C.
  Wire.requestFrom(0x68, 7);                    // Solicitar 7 bytes del DS3231 y liberar el bus I2C al finalizar la lectura.
  second = Wire.read();                         // Leer los segundos desde el registro 0.
  minute = Wire.read();                         // Leer los minutos desde el registro 1.
  hour   = Wire.read();                         // Leer la hora desde el registro 2.
  day    = Wire.read();                         // Leer el día desde el registro 3.
  date   = Wire.read();                         // Leer la fecha desde el registro 4.
  month  = Wire.read();                         // Leer el mes desde el registro 5.
  year   = Wire.read();                         // Leer el año desde el registro 6.
}
void alarms_read_display(){                     // Función para leer y mostrar datos de alarma1, alarma2 y temperatura.
  byte control_reg, temperature_lsb;
  char temperature_msb;
  Wire.beginTransmission(0x68);                 // Iniciar el protocolo I2C con la dirección del DS3231.
  Wire.write(0x08);                             // Enviar la dirección del registro.
  Wire.endTransmission(false);                  // Reinicio del protocolo I2C.
  Wire.requestFrom(0x68, 11);                   // Solicitar 11 bytes del DS3231 y liberar el bus I2C al finalizar la lectura.
  alarm1_minute = Wire.read();                  // Leer los minutos de la alarma 1.
  alarm1_hour   = Wire.read();                  // Leer las horas de la alarma 1.
  Wire.read();                                  // Omitir el registro de día/fecha de la alarma 1.
  alarm2_minute = Wire.read();                  // Leer los minutos de la alarma 2.
  alarm2_hour   = Wire.read();                  // Leer las horas de la alarma 2.
  Wire.read();                                  // Omitir el registro de día/fecha de la alarma 2.
  control_reg = Wire.read();                    // Leer el registro de control del DS3231.
  status_reg  = Wire.read();                    // Leer el registro de estado del DS3231.
  Wire.read();                                  // Omitir el registro de compensación del reg. de aging (envejecimiento).
  temperature_msb = Wire.read();                // Leer el byte más significativo de la temperatura.
  temperature_lsb = Wire.read();                // Leer el byte menos significativo de la temperatura.
    // Convert BCD to decimal
  alarm1_minute = (alarm1_minute >> 4) * 10 + (alarm1_minute & 0x0F);
  alarm1_hour   = (alarm1_hour   >> 4) * 10 + (alarm1_hour & 0x0F);
  alarm2_minute = (alarm2_minute >> 4) * 10 + (alarm2_minute & 0x0F);
  alarm2_hour   = (alarm2_hour   >> 4) * 10 + (alarm2_hour & 0x0F);
    // End conversion
  alarm1[8]     = alarm1_minute % 10  + 48;
  alarm1[7]     = alarm1_minute / 10  + 48;
  alarm1[5]     = alarm1_hour   % 10  + 48;
  alarm1[4]     = alarm1_hour   / 10  + 48;
  alarm2[8]     = alarm2_minute % 10  + 48;
  alarm2[7]     = alarm2_minute / 10  + 48;
  alarm2[5]     = alarm2_hour   % 10  + 48;
  alarm2[4]     = alarm2_hour   / 10  + 48;
  alarm1_status = bitRead(control_reg, 0);      // Leer el bit de habilitación de interrupción de la alarma 1 (A1IE) desde el registro de control del DS3231.
  alarm2_status = bitRead(control_reg, 1);      // Leer el bit de habilitación de interrupción de la alarma 2 (A2IE) desde el registro de control del DS3231.
  if(temperature_msb < 0){
    temperature_msb = abs(temperature_msb);
    temperature[2] = '-';
  }
  else
    temperature[2] = ' ';
  temperature_lsb >>= 6;
  temperature[4] = temperature_msb % 10  + 48;
  temperature[3] = temperature_msb / 10  + 48;
  if(temperature_lsb == 0 || temperature_lsb == 2){
    temperature[7] = '0';
    if(temperature_lsb == 0) temperature[6] = '0';
    else                     temperature[6] = '5';
  }
  if(temperature_lsb == 1 || temperature_lsb == 3){
    temperature[7] = '5';
    if(temperature_lsb == 1) temperature[6] = '2';
    else                     temperature[6] = '7';
  }
  temperature[8]  = 223;                        // El símbolo del grado es "°".
  lcd.setCursor(10, 0);
  lcd.print(temperature);                       // Mostrar temperatura
  lcd.setCursor(0, 2);
  lcd.print(alarm1);                            // Mostrar alarma 1
  lcd.setCursor(17, 2);
  if(alarm1_status)  lcd.print("ON ");          // Si A1IE = 1, imprimir 'ON'.
  else               lcd.print("OFF");          // Si A1IE = 0, imprimir 'OFF'.
  lcd.setCursor(0, 3);
  lcd.print(alarm2);                            // Mostrar alarma 2
  lcd.setCursor(17, 3);
  if(alarm2_status)  lcd.print("ON ");          // Si A2IE = 1, imprimir 'ON'.
  else               lcd.print("OFF");          // Si A2IE = 0, imprimir 'OFF'.
}
void calendar_display(){                        // Función para mostrar el calendario.
  switch(day){
    case 1:  strcpy(calendar, "Dom   /  /20  "); break;
    case 2:  strcpy(calendar, "Lun   /  /20  "); break;
    case 3:  strcpy(calendar, "Mar   /  /20  "); break;
    case 4:  strcpy(calendar, "Mie   /  /20  "); break;
    case 5:  strcpy(calendar, "Jue   /  /20  "); break;
    case 6:  strcpy(calendar, "Vie   /  /20  "); break;
    case 7:  strcpy(calendar, "Sab   /  /20  "); break;
    default: strcpy(calendar, "Sab   /  /20  ");
  }
  calendar[13] = year  % 10 + 48;
  calendar[12] = year  / 10 + 48;
  calendar[8]  = month % 10 + 48;
  calendar[7]  = month / 10 + 48;
  calendar[5]  = date  % 10 + 48;
  calendar[4]  = date  / 10 + 48;
  lcd.setCursor(0, 1);
  lcd.print(calendar);                          // Mostrar calendario
}
void DS3231_display(){
  // Convertir decimal codificado en binario a decimal
  second = (second >> 4) * 10 + (second & 0x0F);
  minute = (minute >> 4) * 10 + (minute & 0x0F);
  hour = (hour >> 4) * 10 + (hour & 0x0F);
  date = (date >> 4) * 10 + (date & 0x0F);
  month = (month >> 4) * 10 + (month & 0x0F);
  year = (year >> 4) * 10 + (year & 0x0F);
  // Fin de la conversión.
  Time[7]     = second % 10  + 48;
  Time[6]     = second / 10  + 48;
  Time[4]     = minute % 10  + 48;
  Time[3]     = minute / 10  + 48;
  Time[1]     = hour   % 10  + 48;
  Time[0]     = hour   / 10  + 48;
  calendar_display();                           // Llama a la función de visualización de calendario.
  lcd.setCursor(0, 0);
  lcd.print(Time);                              // Mostrar hora
}
void Blink(){
  byte j = 0;
  while(j < 10 && (digitalRead(button1) || i >= 5) && digitalRead(button2) && (digitalRead(button3) || i < 5)){
    j++;
    delay(25);
  }
}
byte edit(byte x, byte y, byte parameter){
  char text[3];
  while(!digitalRead(button1) || !digitalRead(button3));    // Esperar hasta que el botón B1 sea liberado.
  while(true){
    while(!digitalRead(button2)){                           // Si el botón B2 es presionado
      parameter++;
      if(((i == 0) || (i == 5)) && parameter > 23)          // Si las horas > 23 ==> hours = 0
        parameter = 0;
      if(((i == 1) || (i == 6)) && parameter > 59)          // Si los minutos > 59 ==> minutes = 0
        parameter = 0;
      if(i == 2 && parameter > 31)                          // Si la fecha presente > 31 ==> date = 1
        parameter = 1;
      if(i == 3 && parameter > 12)                          // Si el mes > 12 ==> month = 1
        parameter = 1;
      if(i == 4 && parameter > 99)                          // Si el año > 99 ==> year = 0
        parameter = 0;
      if(i == 7 && parameter > 1)                           // Para alarmas encendidas o apagadas (1: alarm ON, 0: alarm OFF)
        parameter = 0;
      lcd.setCursor(x, y);
      if(i == 7){                                           // Para alarmas ON & OFF
        if(parameter == 1)  lcd.print("ON ");
        else                lcd.print("OFF");
      }
      else{
        sprintf(text,"%02u", parameter);
        lcd.print(text);
      }
      if(i >= 5){
        DS3231_read();                          // Leer datos del DS3231
        DS3231_display();                       // Mostrar la hora y el calendario del DS3231
      }
      delay(200);                              // Espera 200ms para evitar rebotes
    }
    lcd.setCursor(x, y);
    lcd.print("  ");                            // Dos espacios.
    if(i == 7) lcd.print(" ");                  // Imprimir un espacio seguido (para alarmas ON & OFF)
    Blink();                                    // Llamar a la función Blink.
    lcd.setCursor(x, y);
    if(i == 7){                                 // Para alarmas ON & OFF
      if(parameter == 1)  lcd.print("ON ");
      else                lcd.print("OFF");
    }
    else{
      sprintf(text,"%02u", parameter);
      lcd.print(text);
    }
    Blink();
    if(i >= 5){
      DS3231_read();
      DS3231_display();}
    if((!digitalRead(button1) && i < 5) || (!digitalRead(button3) && i >= 5)){
      i++;                                      // Incrementa 'i' para el siguiente parámetro.
      return parameter;                         // Devolver el valor del parámetro y salir.
    }
  }
}

void setup() {
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(buzzer_pin, LOW);  

  Wire.begin();                                                 // Uniendose al bus i2c  
  Serial.begin(9600);
 
  lcd.init();
  lcd.backlight();                  

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(Alarm);

  delay(500);  
}

void loop() {
  if(!digitalRead(button1)){                    // Si el botón B1 es presionado
      i = 0;
      hour   = edit(0, 0, hour);
      minute = edit(3, 0, minute);
      while(!digitalRead(button1));             // Esperar hasta que el botón B1 sea liberado.
      while(true){
        while(!digitalRead(button2)){           // Si el botón B2 es presionado
          day++;                                // Incrementar el día.
          if(day > 7) day = 1;
          calendar_display();                   // Llamar a la función de mostrar calendario.
          lcd.setCursor(0, 1);
          lcd.print(calendar);                  // Mostrar calendario
          delay(200);
        }
        lcd.setCursor(0, 1);
        lcd.print("   ");                       // Imprimir 3 espacios
        Blink();
        lcd.setCursor(0, 1);
        lcd.print(calendar);                    // Mostrar calendario
        Blink();                                // Llamar a la función Blink.
        if(!digitalRead(button1))               // Si el botón B1 es presionado
          break;
      }
      date = edit(4, 1, date);                  // Editar fecha
      month = edit(7, 1, month);                // Editar mes
      year = edit(12, 1, year);                 // Editar año
      // Convertir decimal a BCD
      minute = ((minute / 10) << 4) + (minute % 10);
      hour = ((hour / 10) << 4) + (hour % 10);
      date = ((date / 10) << 4) + (date % 10);
      month = ((month / 10) << 4) + (month % 10);
      year = ((year / 10) << 4) + (year % 10);
      // fin conversion
      // Write time & calendar data to DS3231 RTC
      Wire.beginTransmission(0x68);             // Iniciar el protocolo I2C con la dirección del DS3231.
      Wire.write(0);                            // Enviar dirección del registro.
      Wire.write(0);                            // Reset sesonds and start oscillator
      Wire.write(minute);                       // Escribe minutos
      Wire.write(hour);                         // Escribe hora
      Wire.write(day);                          // Escribir día.
      Wire.write(date);                         // Escribir fecha
      Wire.write(month);                        // Escribir mes.
      Wire.write(year);                         // Escribir año.
      Wire.endTransmission();                   // Detener la transmisión y liberar el bus I2C.
      delay(200);
    }
    if(!digitalRead(button3)){                  // Si el botón B3 es presionado
      while(!digitalRead(button3));             // Espera hasta que el botón B3 sea liberado.
      i = 5;
      alarm1_hour   = edit(4,  2, alarm1_hour);
      alarm1_minute = edit(7,  2, alarm1_minute);
      alarm1_status = edit(17, 2, alarm1_status);
      i = 5;
      alarm2_hour   = edit(4,  3, alarm2_hour);
      alarm2_minute = edit(7,  3, alarm2_minute);
      alarm2_status = edit(17, 3, alarm2_status);
      alarm1_minute = ((alarm1_minute / 10) << 4) + (alarm1_minute % 10);
      alarm1_hour   = ((alarm1_hour   / 10) << 4) + (alarm1_hour % 10);
      alarm2_minute = ((alarm2_minute / 10) << 4) + (alarm2_minute % 10);
      alarm2_hour   = ((alarm2_hour   / 10) << 4) + (alarm2_hour % 10);
      // Write alarms data to DS3231
      Wire.beginTransmission(0x68);               // Inicia el protocolo I2C con la dirección DS3231.
      Wire.write(7);                              // Enviar dirección del registro (segundos de alarma 1).
      Wire.write(0);                              // Escribir 0 en los segundos de la alarma 1.
      Wire.write(alarm1_minute);                  // Escribir el valor de los minutos de la alarma 1 en el DS3231.
      Wire.write(alarm1_hour);                    // Escribir el valor de las horas de la alarma 1 en el DS3231.
      Wire.write(0x80);                           // La alarma 1 se activa cuando las horas, minutos y segundos coinciden.
      Wire.write(alarm2_minute);                  // Escribir el valor de los minutos de la alarma 2 en el DS3231.
      Wire.write(alarm2_hour);                    // Escribir el valor de las horas de la alarma 2 en el DS3231.
      Wire.write(0x80);                           // La alarma 2 se activa cuando las horas y los minutos coinciden.
      Wire.write(4 | alarm1_status | (alarm2_status << 1));      // Escribir datos en el registro de control del DS3231 (habilitar interrupción cuando suene la alarma).
      Wire.write(0);                              // Borrar los bits de la bandera de la alarma.
      Wire.endTransmission();                     // Detener la transmisión y liberar el bus I2C.
      delay(200);                                 // Espera 200ms
    }
    if(!digitalRead(button2) && digitalRead(buzzer_pin)){         // Cuando el botón B2 es presionado con la alarma activada, se debe restablecer y apagar la alarma.
      digitalWrite(buzzer_pin, LOW);               // Apagar el indicador de alarma.
      digitalWrite(LED_pin, LOW);               // Apagar el indicador de alarma.
      Wire.beginTransmission(0x68);               // Iniciar el protocolo I2C con la dirección del DS3231.
      Wire.write(0x0E);                           // Enviar la dirección del registro (registro de control).
      // Write data to control register (Turn OFF the occurred alarm and keep the other as it is)
      Wire.write(4 | (!bitRead(status_reg, 0) & alarm1_status) | ((!bitRead(status_reg, 1) & alarm2_status) << 1));
      Wire.write(0);                              // Clear alarm flag bits
      Wire.endTransmission();                     // Detener la transmisión y liberar el bus I2C.
    }
    DS3231_read();                                // Leer los parámetros de tiempo y calendario del RTC DS3231.
    alarms_read_display();                        // Leer y mostrar los parámetros de las alarmas.
    DS3231_display();                             // Mostrar hora y calendario
    delay(50);                                    // Esperar 50ms
}

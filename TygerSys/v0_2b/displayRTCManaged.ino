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
  
   /*Control de la temperatura*/

  if(temperature_msb < 0){
    temperature_msb = abs(temperature_msb);
    temperature[2] = '-';
  }
  else
    temperature[2] = ' ';
  temperature_lsb >>= 6;
  temperature[4] = temperature_msb % 10  + 48;
  temperature[3] = temperature_msb / 10  + 48;
  
  /* Por cuestiones de espacio no tratar LSB de la temperatura*/
  /*
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
  */
  temperature[5]  = 223;                        // El símbolo del grado es "°".  
  lcd.setCursor(9, 0);
  lcd.print(temperature);                       // Mostrar temperatura
}
void calendar_display() {                       
  switch (day) {
    case 1:  strcpy(calendar, "Dom     /  /20  "); break;
    case 2:  strcpy(calendar, "Lun     /  /20  "); break;
    case 3:  strcpy(calendar, "Mar     /  /20  "); break;
    case 4:  strcpy(calendar, "Mie     /  /20  "); break;
    case 5:  strcpy(calendar, "Jue     /  /20  "); break;
    case 6:  strcpy(calendar, "Vie     /  /20  "); break;
    case 7:  strcpy(calendar, "Sab     /  /20  "); break;
    default: strcpy(calendar, "Sab     /  /20  ");
  }
  calendar[15] = year  % 10 + 48;
  calendar[14] = year  / 10 + 48;
  calendar[10]  = month % 10 + 48;
  calendar[9]  = month / 10 + 48;
  calendar[7]  = date  % 10 + 48;
  calendar[6]  = date  / 10 + 48;
  calendar[5]  = ' ';
  calendar[4]  = ' ';
  lcd.setCursor(0, 1);
  lcd.print(calendar);                          // Display calendar
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
  // muestra hora y enciende en el modulo de 4 displays de 7Seg
  display.showNumberDecEx(hour, 0b01000000, false, 2, 0); 
  display.showNumberDec(minute, true, 2, 2);    // muestra minutos en formato de 2 dig.
  //delay(100);       
}

void displayRTCManaged(){
    DS3231_read();                                // Leer los parámetros de tiempo y calendario del RTC DS3231.
    alarms_read_display();                        // Leer y mostrar los parámetros de las alarmas.
    DS3231_display();                             // Mostrar hora y calendario
    delay(50);                                    // Esperar 50ms
}    


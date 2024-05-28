
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
    /*Incrementando*/
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
    /* Comentar para la version de  LCD 16x2 */
    /*if(i >= 5){
      DS3231_read();
      DS3231_display();
    }
    */
      delay(200);                               // Espera 200ms para evitar rebotes
    }
  /*Decrementando*/
    while(!digitalRead(button4)){                           // Si el botón B2 es presionado
      parameter--;
        if(((i == 0) || (i == 5)) && parameter == 255)       // Si las horas son < 0 ==> hours = 23
          parameter = 23;                                     
        if(((i == 1) || (i == 6)) && parameter == 255)       // Si los minutos < 0 ==> minutes = 59
          parameter = 59;
        if(i == 2 && parameter < 31)                         // Si la fecha presente > 31 ==> date = 1
          parameter = 1;
        if(i == 3 && parameter > 12)                         // Si el mes > 12 ==> month = 1
          parameter = 1;
        if(i == 4 && parameter == 255)                       // If year < 0 ==> year = 50
          parameter = 50;
        if(i == 7 && parameter == 255)                       // For alarms ON or OFF (1: alarm ON, 0: alarm OFF)
          parameter = 1;         
        lcd.setCursor(x, y);
        if(i == 7){                                           // For alarms print ON  or OFF
          if(parameter == 1)  lcd.print("ON ");
          else                lcd.print("OFF");
        }
        else{
          sprintf(text,"%02u", parameter);
          lcd.print(text);
        }
        delay(200);  
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
    
    /* Comentar para la version de  LCD 16x2 */
    /*if(i >= 5){
      DS3231_read();
      DS3231_display();
    }
    */
    if((!digitalRead(button1) && i < 5) || (!digitalRead(button3) && i >= 5)){
      i++;                                      // Incrementa 'i' para el siguiente parámetro.
      return parameter;                         // Devolver el valor del parámetro y salir.
    }
  }
}

void setDateTime(){
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
      date = edit(6, 1, date);                  // Editar fecha
      month = edit(9, 1, month);                // Editar mes
      year = edit(14, 1, year);                 // Editar año
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
void displayAlarms(){
  // Mostrar alarmas mientras se presiona B2
  while(!digitalRead(button2)){
    while(true){
      lcd.setCursor(0, 0);
      lcd.print(alarm1);                            // Mostrar alarma 1
      lcd.setCursor(13, 0);
      if(alarm1_status)  lcd.print("ON ");          // Si A1IE = 1, imprimir 'ON'.
      else               lcd.print("OFF");          // Si A1IE = 0, imprimir 'OFF'.
      lcd.setCursor(0, 1);
      lcd.print(alarm2);                            // Mostrar alarma 2
      lcd.setCursor(13, 1);
      if(alarm2_status)  lcd.print("ON ");          // Si A2IE = 1, imprimir 'ON'.
      else               lcd.print("OFF");          // Si A2IE = 0, imprimir 'OFF'.
        if(digitalRead(button2)) break;
      }
  }  
}
void setAlarms(){
  while(!digitalRead(button3));             // Espera hasta que el botón B3 sea liberado.      
  /* Se visualizan las alarmas mientras se editan*/
  lcd.setCursor(0, 0);
  lcd.print(alarm1);                           // Display alarm1
  lcd.setCursor(13, 0);
  if (alarm1_status)  lcd.print("ON ");        // If A1IE = 1 print 'ON'
  else                lcd.print("OFF");        // If A1IE = 0 print 'OFF'
  lcd.setCursor(0, 1);
  lcd.print(alarm2);                           // Display alarm2
  lcd.setCursor(13, 1);
  if (alarm2_status)  lcd.print("ON ");        // If A2IE = 1 print 'ON'
  else                lcd.print("OFF");        // If A2IE = 0 print 'OFF'         
  i = 5;
  alarm1_hour   = edit(4,  0, alarm1_hour);
  alarm1_minute = edit(7,  0, alarm1_minute);
  alarm1_status = edit(13, 0, alarm1_status);
  i = 5;
  alarm2_hour   = edit(4,  1, alarm2_hour);
  alarm2_minute = edit(7,  1, alarm2_minute);
  alarm2_status = edit(13, 1, alarm2_status);
  alarm1_minute = ((alarm1_minute / 10) << 4) + (alarm1_minute % 10);
  alarm1_hour   = ((alarm1_hour   / 10) << 4) + (alarm1_hour % 10);
  alarm2_minute = ((alarm2_minute / 10) << 4) + (alarm2_minute % 10);
  alarm2_hour   = ((alarm2_hour   / 10) << 4) + (alarm2_hour % 10);
  // Escribe los datos de la alarma al DS3231
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
void offAlarms(){
  digitalWrite(buzzer_pin, LOW);              // Apagar el indicador de alarma.
  digitalWrite(LED_pin, LOW);                 // Apagar el indicador de alarma.
  Wire.beginTransmission(0x68);               // Iniciar el protocolo I2C con la dirección del DS3231.
  Wire.write(0x0E);                           // Enviar la dirección del registro (registro de control).
  // Escribir datos en el registro de control (Apagar la alarma ocurrida y mantener las otras como están)
  Wire.write(4 | (!bitRead(status_reg, 0) & alarm1_status) | ((!bitRead(status_reg, 1) & alarm2_status) << 1));
  Wire.write(0);                              // Clear alarm flag bits
  Wire.endTransmission();                     // Detener la transmisión y liberar el bus I2C.
}

void operationModeManaged(int mode){
  switch (mode){
    case NULLSTATE:
      displayRTCManaged();
    break;
    case SET_DATETIME:
      setDateTime();
    break;    
    case DISPLAY_ALARMS:
      displayAlarms();
    break;
    case SET_ALARMS:
      setAlarms();
    break;
    case OFF_ALARMS:
      offAlarms();
    break;  
    case FOOD_DELIVERY:
       openGate();
       closeGate();   
       offAlarms();    
    break;  

    default: break;    
  }
}      



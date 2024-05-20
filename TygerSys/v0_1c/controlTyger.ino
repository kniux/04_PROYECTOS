void testButtons(){
  if(!digitalRead(button1)){                  // B1 es presionado
      systemsate =  SET_DATETIME;      
    }
  if(!digitalRead(button2)){                  // B2 es presionado
      systemsate =  DISPLAY_ALARMS;      
  }
  if(!digitalRead(button3)){                  // B3 es presionado
      systemsate =  SET_ALARMS;      
  }
  if(!digitalRead(button2) && digitalRead(buzzer_pin)){   // B2 es presionado con la alarma activa, se restablecer y apaga la alarma.
      systemsate =  OFF_ALARMS;      
  }
}

void controlTyger(){
  testButtons();
  operationModeManaged(systemsate); 

  if (systemsate != NULLSTATE) systemsate =  NULLSTATE;    
}

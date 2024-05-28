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
 
 /* De acuerdo con el nuevo diseño ¡El sistema nunca entrará a este escenario! 
  * Por lo que podemos emitirlo 
  if(!digitalRead(button2) && digitalRead(buzzer_pin)){   
      systemsate =  OFF_ALARMS;      
  }
*/ 
  if(digitalRead(buzzer_pin)){   // alarma activa
      systemsate =  FOOD_DELIVERY;      
  }  
}

void controlTyger(){
  testButtons();
  operationModeManaged(systemsate); 
  
  if (systemsate != NULLSTATE) systemsate =  NULLSTATE;    
}

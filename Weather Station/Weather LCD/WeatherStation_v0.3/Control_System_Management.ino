/*
 * Control_System_Management
 */

 void FSM_System_Transitions(){
  unsigned long currentMillis = millis();
  
  if (((currentMillis - previousMillisState1) >= Timeinterval_1 )  // Revisando que se cumpla la condición
        && (systemState == S_HOME)){   //Forzando a realizar la comparación con el estado IDLE
    previousMillisState1 = currentMillis;
    systemState = S_REFRESH_SCREEN;
  }
  if (((currentMillis - previousMillisState2) >= Timeinterval_2 )
        && (systemState == S_HOME)){
    previousMillisState2 = currentMillis;
    systemState = S_READ_SENSORS;
  }
  if (((currentMillis - previousMillisState3) >= Timeinterval_3 )
        && (systemState == S_HOME)){
    previousMillisState3 = currentMillis;
    systemState = S_SWITCH_SCREEN;
  }
}

 void FSM_System(){
  switch(systemState){
    case S_HOME:              /*STATE 0: NO SE EJECUTA TAREA*/
    break;
    case S_REFRESH_SCREEN:    /*STATE 1: EJECUTA TAREA REFREZCO DE PANTALLA*/
        RefreshScreen();
    break;
    case S_READ_SENSORS:      /*STATE 2: EJECUTA TAREA LECTURA DE SENSORES*/
        ReadSensors();
    break;
    case S_SWITCH_SCREEN:     /*STATE 3: EJECUTA TAREA INTERCAMBIO DE PANTALLA ACTIVA*/
        FSM_Screens();
    break;    
    default:
    break;  
  }
  // Regresando el estado de la FSM_System al idle (S_HOME)
  if(systemState != S_HOME)
      systemState = S_HOME;
}

/*
 * Weather_Transition_Management
 */
 // Subsistema de Gestión de Transiciones para la FSM del sistema **************************************/
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

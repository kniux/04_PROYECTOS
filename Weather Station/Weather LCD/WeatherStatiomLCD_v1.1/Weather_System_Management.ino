/*
 * Weather_System_Management
 */
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
void RefreshScreen(){
  switch(screenState){
    case SCREEN_WELCOME:    // Refrezco bienvenida (no cambia)
        displayWelcome();
    break;
    case SCREEN_DATEHOUR:   // Refrezco fecha-hora (se nota el cambio en los segundos)
        displayDate();
        displayHour();
    break;
    case SCREEN_HUMTEMP:
        displayHumtemp();  // Refrezco humedad-temp (se nota el cambio, si lo hay, cada 2seg)
    break;
    default:
    break;
  }
}

/*
 * Sensor_Management
 */
 void ReadSensors(){
  tempDHT = dht11.readTemperature();
  Hum = dht11.readHumidity();
  tempDS = rtc.getTemperature();
}
void FSM_Screens(){
  switch(screenState){
     case SCREEN_WELCOME:
        cleanScreen();        
        screenState = SCREEN_DATEHOUR;
    break;
    case SCREEN_DATEHOUR:
       cleanScreen();
        screenState = SCREEN_HUMTEMP;
    break;
    case SCREEN_HUMTEMP:
       cleanScreen();
       screenState = SCREEN_DATEHOUR;
    break;
    default:
    break;
  }
}

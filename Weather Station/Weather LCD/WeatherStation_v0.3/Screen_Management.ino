/*
   Screen_Management
*/
// Subsistema de Gestión de Pantallas  *************************************/

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

void displayWelcome() {
  lcd.setCursor(0, 0);
  lcd.print("  Weather Station") ;
  lcd.setCursor(5, 1);
  lcd.print("Ver. 1.0") ;
}
void displayDate() {
  DateTime now = rtc.now();
  char _bufferFecha[12];
  lcd.setCursor(0, 0);
  formatoFecha(_bufferFecha, now.day(), now.month(), now.year() );
  lcd.print(_bufferFecha);
}
void displayHour() {
  DateTime now = rtc.now();
  char _bufferHora[10];
  lcd.setCursor(0, 1);
  formatoHora( _bufferHora, now.hour(), now.minute(), now.second() );
  lcd.print(_bufferHora);
}
void formatoFecha(char bufferFecha[12], int numDia, int numMes, int numA) {
  sprintf(bufferFecha, "%02d/%02d/%04d", numDia, numMes, numA);
}
void formatoHora(char bufferHora[10], int hora, int minu, int seg) {
  sprintf(bufferHora, "%02d:%02d:%02d", hora, minu, seg);
}
void displayHumtemp() {
  char symbol = 223;
  lcd.setCursor(0, 0);
  lcd.print("DH:");
  lcd.print(tempDHT);
  lcd.print(symbol);
  lcd.print(" T:");
  lcd.print(tempDS);
  lcd.print(symbol);
  lcd.print('C');
  lcd.setCursor(0, 1);
  lcd.print("Lic.En Informatica");
 // lcd.setCursor(9, 1);
 // lcd.print(Hum);
 // lcd.print("%");

  Serial.print("DH: ");
  Serial.print(tempDHT);
  Serial.println("°");
  Serial.print(" DS: ");
  Serial.print(tempDS);
  Serial.println("°");

  Serial.print("HUMEDAD: ");
  Serial.print(Hum);
  Serial.println("%");
}
void cleanScreen() {
  lcd.clear();
}

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

void setGPIO(){
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(buzzer_pin, OUTPUT);  
}
void setSensorsActuators(){
  setGPIO();
  digitalWrite(buzzer_pin, LOW);  
  lcd.init();
  lcd.backlight();                  
}
void initInterrupts(){
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(Alarm);
}
void setComm(){
  Wire.begin();                                                 
  Serial.begin(9600);
}

void setupManaged(){
  setSensorsActuators();
  initInterrupts();
  setComm();
  delay(500);   
}

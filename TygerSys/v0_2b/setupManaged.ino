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
  pinMode(button4, INPUT_PULLUP);
  pinMode(buzzer_pin, OUTPUT);  
}
void ServoCalibration(){
  lcd.setCursor(2, 0);
  lcd.print("Calibration...");
  for (pos = 0; pos <= 180; pos += 1) { // "Va desde 0 grados hasta 180 grados".
    // in steps of 1 degree
    myservo.write(pos);              // Se le indica al servo que vaya a la posición almacenada en la variable 'pos'.
    delay(15);                       // Espera 15 ms para que el servo alcance la posición.
  }
  for (pos = 180; pos >= 0; pos -= 1) { // Va desde 180 grados hasta 0 grados.
    myservo.write(pos);              
    delay(15);                       
  }
  lcd.setCursor(8, 1);
  lcd.print("OK");
}
void setSensorsActuators(){
  setGPIO();
  digitalWrite(buzzer_pin, LOW);  
  lcd.init();
  lcd.backlight();        
  myservo.attach(7);  
  display.setBrightness(7);
  display.setSegments(data); // Enciende displays
  ServoCalibration();
	display.setSegments(done); // Se meustra "dOnE" en los  displays
}
void initInterrupts(){
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(Alarm);
}
void setComm(){
  Wire.begin();                                                 
  Serial.begin(9600);
}
void controlSM(){
  setSensorsActuators();
  initInterrupts();
  setComm();
}
void setupManaged(){
  controlSM();  
  delay(500);   
}

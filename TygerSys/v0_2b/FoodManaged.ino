void openGate(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Liberando comida ");
  for (pos = 0; pos <= 180; pos += 1) { // "Va desde 0 grados hasta 180 grados".
    // in steps of 1 degree
    myservo.write(pos);              // Se le indica al servo que vaya a la posición almacenada en la variable 'pos'.
    delay(15);                       // Espera 15 ms para que el servo alcance la posición.
  }
}

void closeGate(){
  //Función de reconocimiento de llenado de contenedor
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Cerrando compuerta");
  for (pos = 180; pos >= 0; pos -= 1) { // Va desde 180 grados hasta 0 grados.
    myservo.write(pos);              
    delay(15);                       
  }
  lcd.clear();
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("Compuerta cerrada");
  delay(500);
  lcd.clear();
}

  
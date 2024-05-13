/*
 * Sensor_Management
 */
 void ReadSensors(){
  tempDHT = dht11.readTemperature();
  Hum = dht11.readHumidity();
  tempDS = rtc.getTemperature();
}

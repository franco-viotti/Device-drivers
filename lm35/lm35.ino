//Paso 1
const int sensorPin = A0;
 
void setup()
{
  Serial.begin(9600);
}
 
void loop()
{
  //Paso 2
  float valueAdc = analogRead(sensorPin);
  //Serial.println(valueAdc);
  Serial.println(valueAdc);
  
  //Paso 3
  //float volts = (valueAdc / 1023.0) * 5;
  //Serial.println(volts);

  //Paso 4
  //float celsius = volts * 100.00; 
  //Paso 5
  //Serial.print(celsius);
  //Serial.println(" C");
  delay(1000);
}

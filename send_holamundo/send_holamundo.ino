String nom = "Arduino";
String msg = "";
const int lm35_pin = A0;

void setup() {
   Serial.begin(9600);
}

void loop() {
  sendData();
  delay(500);
}

void sendData() {
  // Escribir datos
  Serial.print(nom);
  Serial.print(" celsius read: ");
  getCelsius();
  Serial.println(msg);
}

void getCelsius(){
  float valueADC = analogRead(lm35_pin);
  float volts = (valueADC / 1023.0) * 5;
  float celsius = volts * 100.00;
  msg = String(celsius);   
}
  

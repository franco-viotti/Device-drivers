String nom = "Arduino";
String msg;
const int lm35_pin = A0;

void setup() {
   Serial.begin(9600);
}

void loop() {
  readLM35();
  if (msg != "") {
      sendData();
  }
  delay(500);
}

void readLM35() {
  msg = "";
  float valueADC = analogRead(lm35_pin);
  float volts = (valueADC / 1023.0) * 5;
  float celsius = volts * 100.00;
  msg = "La cantidad de celsius es: " + String(celsius); 
  msg = "Hola Mundo";
}

void sendData() {
  //write data
  Serial.print(nom);
  Serial.print(" received : ");
  Serial.print(msg);
}

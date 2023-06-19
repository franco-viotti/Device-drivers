#include <DHT.h>
#include <DHT_U.h>

String nom = "Arduino";
String msg = "";
const int lm35_pin = A0;
const int SENSOR = 2; // pin OUT de DHT11 a pin digital 2
int HUM;

DHT dht(SENSOR, DHT11); // creación del objeto

void setup() {
   Serial.begin(9600); // inicialización del monitor serial
   dht.begin(); // inicialización de sensor
}

void loop() {
  sendData();
  delay(500);
}

void sendData() {
  // Escribir datos
  Serial.print(nom);
  Serial.print(" celsius read: ");
  readLM35();
  Serial.println(msg);
  Serial.print("\n");
  Serial.print(nom);
  Serial.print(" humidity read: ");
  readDH11();
  Serial.println(msg);
  Serial.print("\n");
}

void readLM35(){
  float valueADC = analogRead(lm35_pin);
  float volts = (valueADC / 1023.0) * 5;
  float celsius = volts * 100.00;
  msg = String(celsius);   
}
 
void readDH11(){
  HUM = dht.readHumidity(); // se obtiene el valor de la humedad
  msg = HUM;
}

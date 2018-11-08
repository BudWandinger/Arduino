#include "DHT.h";

#define HumidityGPIO 4

DHT humiditySensor(HumidityGPIO, DHT22);

void setup() {
  Serial.begin(9600);
  humiditySensor.begin();
}

void loop() 
{
  float temp = humiditySensor.readTemperature();
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" = ");
  Serial.println(uint32_t(temp));

  float hum = humiditySensor.readHumidity();
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" = ");
  Serial.println(uint32_t(hum));
  
  delay(1000);
}

#include "DHT.h";

#define INPUT_HUMIDITY_SENSOR_GPIO 2
#define OUTPUT_HUMIDITY_SENSOR_GPIO 3
#define FAN_POWER_GPIO 4
#define FAN_CONTROL_GPIO 5

DHT inputHumiditySensor(INPUT_HUMIDITY_SENSOR_GPIO, DHT22);
DHT outputHumiditySensor(OUTPUT_HUMIDITY_SENSOR_GPIO, DHT22);

void setup() 
{
  Serial.begin(9600);
  
  inputHumiditySensor.begin();
  outputHumiditySensor.begin();

  pinMode(FAN_POWER_GPIO, OUTPUT);
  digitalWrite(FAN_POWER_GPIO, HIGH);

  pinMode(FAN_CONTROL_GPIO, OUTPUT);
  analogWrite(FAN_CONTROL_GPIO, 255);
  delay(2000);
  analogWrite(FAN_CONTROL_GPIO, 0);
  delay(2000);
  for(int i = 5; i <= 255; i += 5)
  {
    analogWrite(FAN_CONTROL_GPIO, i);
    delay(100);
  }
}

void loop() 
{
  Serial.print("Input Temperature: ");
  Serial.println(inputHumiditySensor.readTemperature());
  Serial.print("Input Humidity: ");
  Serial.println(inputHumiditySensor.readHumidity());
  Serial.print("Output Temperature: ");
  Serial.println(outputHumiditySensor.readTemperature());
  Serial.print("Output Humidity: ");
  Serial.println(outputHumiditySensor.readHumidity());
  Serial.println();

  delay(2000);
}

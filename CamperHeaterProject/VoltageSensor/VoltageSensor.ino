
void setup() 
{
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() 
{
  double voltageSum = 0;
  for(int i = 0; i < 100; i++)
  {
    double analogIn = analogRead(A0);
    double percent = analogIn/1023;
    double voltage = percent * 5.0; 
    voltageSum += voltage;
  }
  double averageVoltage = voltageSum/100;
  
  Serial.println(averageVoltage);
  
  delay(500);
}

void setup() {
  pinMode(5, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  for(int i = 0; i <= 250; i+=50)
  {
    analogWrite(5, i);
    Serial.print("Speed: ");
    Serial.println(i);
    delay(10000);
  }
}

int counter = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Hello World!");
  delay(1000);
}

void loop() {
  Serial.println(counter++);
  delay(1000);
}

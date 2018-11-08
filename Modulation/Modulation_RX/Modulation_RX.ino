#define data A0
#define att A1

void setup() {
  Serial.begin(9600);
  pinMode(data, INPUT);
  pinMode(att, INPUT);
}

void loop() {
  int msg[8];
  while (digitalRead(att) == HIGH);
  for (int i = 0; i < 8; i++) {
    msg[i] = digitalRead(data);
    delay(1);
  }
  Serial.print("MSG RECIEVED: ");
  for (int i = 0; i < 8; i++) {
    if (msg[i] == 0)
      Serial.print(0);
    else if (msg[i] == 1)
      Serial.print(1);

    Serial.print(" ");
  }
  Serial.println();
}

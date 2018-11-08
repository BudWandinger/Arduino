#define data A0
#define att A1

int msg[] = {1, 1, 0, 0, 1, 0, 1, 0};

void setup() {
  pinMode(data, OUTPUT);
  pinMode(att, OUTPUT);
  digitalWrite(att, HIGH);
}

void loop() {
  delay(1000);
  digitalWrite(att, LOW);
  for(int i = 0; i < 8; i++){
    digitalWrite(data, msg[i]);
    delay(1);
  }
  digitalWrite(att, HIGH);
  delay(1000);
  
}

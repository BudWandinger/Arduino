int enA = 10;
int in1 = 2;
int in2 = 3;

int enB = 11;
int in3 = 4;
int in4 = 5;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 200);
  delay(5000);
  analogWrite(enA, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 200);
  delay(5000);
  analogWrite(enA, 0);

  for (int i = 0; i < 250; i++) {
    analogWrite(enA, i);
    delay(50);
  }
  for(int i = 250; i >=0; i--){
      analogWrite(enA, i);
      delay(50);
    }
  while (1);
}

#define A 22
#define B 23
#define C 24
#define D 25
#define E 26
#define F 27
#define K 28
#define X A1
#define Y A2

void setup() {
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(C, INPUT);
  pinMode(D, INPUT);
  pinMode(E, INPUT);
  pinMode(F, INPUT);
  pinMode(K, INPUT);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);
  Serial.begin(9600);
}

void loop() {
  /*Serial.print("A: ");
  Serial.println(digitalRead(A));
  
  Serial.print("B: ");
  Serial.println(digitalRead(B));
  
  Serial.print("C: ");
  Serial.println(digitalRead(C));
  
  Serial.print("D: ");
  Serial.println(digitalRead(D));
  
  Serial.print("E: ");
  Serial.println(digitalRead(E));
  
  Serial.print("F: ");
  Serial.println(digitalRead(F));

  Serial.print("K: ");
  Serial.println(digitalRead(K));
*/
  //Serial.print("X: ");
  //Serial.println(analogRead(X));

  Serial.print("Y: ");
  Serial.println(analogRead(Y));

  delay(100);
}

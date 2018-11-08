#define DIAL A0
#define RED 2
#define GREEN 3
#define BLUE 4

void setup() {
  pinMode(DIAL, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned int voltage = analogRead(DIAL);
  Serial.println(voltage);
  delay(50);
  //for(int i = 0; i <= 255; i++){
  //  analogWrite(RED, i);
  //  delay(10);
  //}
  if(voltage >= 0 && voltage <=170){
    analogWrite(RED, 255);
    analogWrite(GREEN, voltage*1.5);
    analogWrite(BLUE, 0);
  }
  else if(voltage > 170 && voltage <= 341){
    analogWrite(RED, (340 - voltage)*1.5);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 0);
  }
  else if(voltage > 341 && voltage <= 511){
    analogWrite(RED, 0);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, (voltage - 341)*1.5);
  }
  else if(voltage > 511 && voltage <= 682){
    analogWrite(RED, 0);
    analogWrite(GREEN, (682 - voltage)*1.5);
    analogWrite(BLUE, 255);
  }
  else if(voltage > 682 && voltage <= 852){
    analogWrite(RED, (voltage - 682)*1.5);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 255);
  }
  else if(voltage > 852 && voltage <= 1023){
    analogWrite(RED, 255);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, (1023 - voltage)*1.5);
  }
}


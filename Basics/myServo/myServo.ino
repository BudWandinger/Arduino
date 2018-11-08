int pin = 44;
#include <Servo.h>
Servo myServo;
void setup() {
  myServo.attach(pin);
  Serial.begin(9600);
}

void loop() {
  myServo.write(0);
  for (int i = 0; i < 180; i++) {
    //Serial.println(i);
    myServo.write(i);
    delay(1);
  }
  for(int i = 180; i > 0; i--){
    myServo.write(i);
    delay(1);
  }

}

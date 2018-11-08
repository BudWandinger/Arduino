#include <Servo.h>

#define GRABBER_PIN 2
#define GRABBER_MIN 76
#define GRABBER_MAX 155
Servo grabber;

void setup() {
  Serial.begin(9600);
  
  grabber.attach(GRABBER_PIN); 
  grabber.write(GRABBER_MAX);
 // delay(1000);
 // grabber.write(GRABBER_MIN);

  Serial.println(grabber.read());
}

void loop() {
  /*
  grabber.write(GRABBER_MAX);
  delay(500);
  grabber.write(GRABBER_MIN);
  delay(500);
  */
}

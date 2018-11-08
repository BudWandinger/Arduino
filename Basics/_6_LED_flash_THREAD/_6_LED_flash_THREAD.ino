using namespace std;
#include <Servo.h>

#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5
#define LED5 6
#define LED6 7

#define BUTTON1 52
#define BUTTON2 53

/*********************************************************/
class flashAll_THREAD {
  public:
    flashAll_THREAD();
    void run(void);
  private:
    int STATE;
    int i;
    int ms;
};
flashAll_THREAD::flashAll_THREAD(void) {
  STATE = 0;
  i = 0;
  ms = 0;
}
void flashAll_THREAD::run(void) {
  if (digitalRead(BUTTON1) == LOW)
    return;

  int thisMS = millis();
  if (thisMS == ms)
    return;
  else
    ms = thisMS;

  if (i == 6)
    i = 0;

  if (thisMS % 100 == 0 && STATE == 0) {
    digitalWrite(i + 2, HIGH);
    STATE = 1;
  }

  else if (thisMS % 100 == 0 && STATE == 1) {
    digitalWrite(i + 2, LOW);
    STATE = 0;
    i++;
  }

  return;
}
/**************************************************************/

class flashColors_THREAD {
  public:
    flashColors_THREAD();
    void run(void);
  private:
    int STATE;
    int i;
    int ms;
};
flashColors_THREAD::flashColors_THREAD(void) {
  STATE = 0;
  i = 0;
  ms = 0;
}
void flashColors_THREAD::run(void) {
  if (digitalRead(BUTTON2) == LOW)
    return;

  int thisMS = millis();
  if (thisMS == ms)
    return;
  else
    ms = thisMS;

  if (i == 3)
    i = 0;

  if (thisMS % 100 == 0 && STATE == 0) {
    digitalWrite(i + 2, HIGH);
    digitalWrite(i + 5, HIGH);
    STATE = 1;
  }
  else if (thisMS % 100 == 0 && STATE == 1) {
    digitalWrite(i + 2, LOW);
    digitalWrite(i + 5, LOW);
    STATE = 0;
    i++;
  }

  return;
}
/**************************************************************/
class servo_THREAD {
  public:
    servo_THREAD();
    void run(Servo);
  private:
    int STATE;
    int ms;
};
servo_THREAD::servo_THREAD(void) {
  STATE = 0;
  ms = 0;
}
void servo_THREAD::run(Servo myServo) {
  int thisMS = millis();
  if (thisMS == ms)
    return;
  else
    ms = thisMS;

  if (STATE == 180)
    STATE = 0;

  if (thisMS % 100 == 0) {
    myServo.write(STATE);
    STATE = STATE + 10;
  }
}
/**************************************************************/
void flashAll() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(i + 2, HIGH);
    delay(50);
    digitalWrite(i + 2, LOW);
  }
  return 0;
}


void flashColors() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(i + 2, HIGH);
    digitalWrite(i + 5, HIGH);
    delay(100);
    digitalWrite(i + 2, LOW);
    digitalWrite(i + 5, LOW);
  }
  return;
}

Servo myservo;
flashAll_THREAD flashAllThread;
flashColors_THREAD flashColorsThread;
servo_THREAD servoThread;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);

  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);

  myservo.attach(9);//attachs the servo on pin 9 to servo object
  myservo.write(0);//back to 0 degrees
  delay(1000);//wait for a second
}

void loop() {
  flashAllThread.run();
  flashColorsThread.run();
  //servoThread.run(myservo);
  //myservo.write(10);//back to 0 degrees
}

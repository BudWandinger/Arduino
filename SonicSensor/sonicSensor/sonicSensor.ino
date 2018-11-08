#include <Servo.h>

#define ON 1
#define OFF 0
#define INCREASE 1
#define DECREASE 0
#define MOTOR_IN1 A2
#define MOTOR_IN2 A3
#define MOTOR_IN3 A4
#define MOTOR_IN4 A5
#define MOTOR_ENA 5
#define MOTOR_ENB 6
#define SERVO_PIN 3
#define MIN_SERVO_ANGLE 40
#define MAX_SERVO_ANGLE 180
#define SERVO_INCREMENT 20
#define SERVO_DELAY 50
#define SS_TRIG A1
#define SS_ECHO A0

int servoDirection = INCREASE;
int servoAngle = MIN_SERVO_ANGLE;
const int SERVO_MIDDLE = MIN_SERVO_ANGLE + ((MAX_SERVO_ANGLE - MIN_SERVO_ANGLE) / 2);

/**********************************************************/

class SonicSensor {
  public:
    SonicSensor(int, int, long);
    long distance();
    int serialMonitor(int);
  private:
    long margin;
    long recentDistances[10];
    long *head;
    long averageDistance;
    bool withinMargin(long);
    int SS_trig;
    int SS_echo;
    int serialMonitorMode;
};

SonicSensor::SonicSensor(int SS_trig, int SS_echo, long margin) {
  for (int i = 0; i < 10; i++)
    recentDistances[i] = i;
  averageDistance = 0;
  head = recentDistances;
  this->margin = margin;
  this->SS_trig = SS_trig;
  this->SS_echo = SS_echo;
  serialMonitorMode = OFF;
  pinMode(SS_trig, OUTPUT);
  pinMode(SS_echo, INPUT);
}

bool SonicSensor::withinMargin(long thisDistance)
{
  long previousAverage = averageDistance;

  if (head == &recentDistances[0])
    head = recentDistances + 9;
  else
    head--;

  *head = thisDistance;

  averageDistance = 0;
  for (int i = 0; i < 10; i ++) {
    averageDistance += recentDistances[i];
  }
  averageDistance = averageDistance / 10;

  if (thisDistance < previousAverage + margin ||
      thisDistance > previousAverage - margin)
    return false;
  else
    return true;
}

long SonicSensor::distance()
{
  bool foo;
  long distance, duration;
  do {
    digitalWrite(SS_trig, LOW);
    delayMicroseconds(2);
    digitalWrite(SS_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(SS_trig, LOW);
    duration = pulseIn(SS_echo, HIGH, 80000);
    distance = (duration / 2) / 29.1;
    foo = withinMargin(distance);
  } while (foo);
  if (serialMonitorMode == ON) {
    Serial.print("Distance: ");
    Serial.println(distance);
  }
  return distance;
}

int SonicSensor::serialMonitor(int mode)
{
  if (mode == ON) {
    Serial.begin(9600);
    serialMonitorMode = ON;
    return 1;
  }
  else if (mode == OFF) {
    Serial.end();
    serialMonitorMode = OFF;
    return 1;
  }
  else return 0;
}

/*******************************************************/

void goForward()
{
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
  analogWrite(MOTOR_ENA, 255);
  analogWrite(MOTOR_ENB, 255);
}

void stopMoving()
{
  analogWrite(MOTOR_ENA, LOW);
  analogWrite(MOTOR_ENB, LOW);
}

void turnLeft()
{
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
  analogWrite(MOTOR_ENA, 255);
  analogWrite(MOTOR_ENB, 255);
}

void turnRight()
{
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
  analogWrite(MOTOR_ENA, 255);
  analogWrite(MOTOR_ENB, 255);
}

/*******************************************************/

void advanceServo(Servo servo)
{
  if (servoAngle == MAX_SERVO_ANGLE)
    servoDirection = DECREASE;
  else if (servoAngle == MIN_SERVO_ANGLE)
    servoDirection = INCREASE;

  if (servoDirection == INCREASE)
    servoAngle += SERVO_INCREMENT;
  else if (servoDirection == DECREASE)
    servoAngle -= SERVO_INCREMENT;

  servo.write(servoAngle);
  delay(SERVO_DELAY);
}

/*******************************************************/

SonicSensor ss(SS_TRIG, SS_ECHO, 20);
Servo servo;

void setup() {
  //ss.serialMonitor(ON);
  servo.attach(SERVO_PIN);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);
  pinMode(MOTOR_ENB, OUTPUT);
}
void loop() {
  long distance;
  
  goForward();
  do {
    distance = ss.distance();
    advanceServo(servo);
  } while (distance > 35 || distance == 0);
  stopMoving();
  
  if (servoAngle <= SERVO_MIDDLE)
    turnLeft();
  else if (servoAngle > SERVO_MIDDLE)
    turnRight();
  do {
    distance = ss.distance();
    //advanceServo(servo);
  } while (distance < 100);
  stopMoving();
}

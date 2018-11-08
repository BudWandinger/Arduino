#include <SPI.h>
#include "RF_COMM.h"
#include "PS2_CONTROL.h"

#define MOTOR_IN1 A2
#define MOTOR_IN2 A3
#define MOTOR_IN3 A4
#define MOTOR_IN4 A5
#define MOTOR_ENA 5
#define MOTOR_ENB 6

unsigned char adr[5] = {0x34, 0x43, 0x10, 0x10, 0x01};
RF_COMM rf(5, adr, 9);

PS2_CONTROL ps2_con;

void setup() {
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);
  pinMode(MOTOR_ENB, OUTPUT);

  rf.initialize(9, 8, 10);
  SPI.begin();
  rf.RX_Mode();

}

void loop() {
  rf.RX();
  if (rf.get_RX_BUFFER() != NULL) {

    ps2_con.setBuffer(rf.get_RX_BUFFER());
    rf.clear_RX_BUFFER();

    ps2_con.decodeBuffer();
    ps2_con.clearBuffer();
    
    if (ps2_con.getDpadUp())
      fullForward();
    else if (ps2_con.getDpadDown())
      fullBackward();
    else if (ps2_con.getDpadRight())
      fullRight();
    else if (ps2_con.getDpadLeft())
      fullLeft();
    else
      fullStop();

    //driveMotors(ps2_con.getLeftJoyX(), ps2_con.getLeftJoyY());




    /*
        Serial.begin(9600);

        Serial.print("Left X: ");
        Serial.print(ps2_con.getLeftJoyX());
        Serial.print(", ");

        Serial.print("Left Y: ");
        Serial.print(ps2_con.getLeftJoyY());
        Serial.print(", ");

        Serial.print("Right X: ");
        Serial.print(ps2_con.getRightJoyX());
        Serial.print(", ");

        Serial.print("Right Y: ");
        Serial.print(ps2_con.getRightJoyY());
        Serial.print(", ");

        Serial.println();

        if (ps2_con.getSelect())
          Serial.println("SELECT");
        if (ps2_con.getLeftJoyButton())
          Serial.println("LEFT JOY BUTTON");
        if (ps2_con.getRightJoyButton())
          Serial.println("RIGHT JOY BUTTON");
        if (ps2_con.getStart())
          Serial.println("START");
        if (ps2_con.getDpadUp())
          Serial.println("DPAD UP");
        if (ps2_con.getDpadRight())
          Serial.println("DPAD RIGHT");
        if (ps2_con.getDpadDown())
          Serial.println("DPAD DOWN");
        if (ps2_con.getDpadLeft())
          Serial.println("DPAD LEFT");
        if (ps2_con.getL2())
          Serial.println("L2");
        if (ps2_con.getR2())
          Serial.println("R2");
        if (ps2_con.getL1())
          Serial.println("L1");
        if (ps2_con.getR1())
          Serial.println("R1");
        if (ps2_con.getTriangle())
          Serial.println("TRIANGLE");
        if (ps2_con.getCircle())
          Serial.println("CIRCLE");
        if (ps2_con.getCross())
          Serial.println("CROSS");
        if (ps2_con.getSquare())
          Serial.println("SQUARE");

        Serial.end();
    */
  }

}

void fullForward()
{
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
  analogWrite(MOTOR_ENA, 255);
  analogWrite(MOTOR_ENB, 255);
}
void fullBackward()
{
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
  analogWrite(MOTOR_ENA, 255);
  analogWrite(MOTOR_ENB, 255);
}
void fullRight()
{
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
  analogWrite(MOTOR_ENA, 255);
  analogWrite(MOTOR_ENB, 255);
}
void fullLeft()
{
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
  analogWrite(MOTOR_ENA, 255);
  analogWrite(MOTOR_ENB, 255);
}

void fullStop()
{
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, LOW);
  analogWrite(MOTOR_ENA, 0);
  analogWrite(MOTOR_ENB, 0);
}
void driveMotors(byte x, byte y)
{
  Serial.begin(9600);
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(", y: ");
  Serial.println(y);

  byte leftMotor;
  byte rightMotor;

  // both motors going forward
  if (y > 127) {
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, HIGH);

    leftMotor = y + x;
    if (leftMotor > 255)
      leftMotor = 255;
    if (leftMotor < 0)
      leftMotor = 0;

    rightMotor = y - x;
    if (rightMotor > 255)
      rightMotor = 255;
    if (rightMotor < 0);
    rightMotor = 0;
  }
  // both motors going backwards
  else if (y < 127) {
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, HIGH);
    digitalWrite(MOTOR_IN3, HIGH);
    digitalWrite(MOTOR_IN4, LOW);

    leftMotor = (127 - y) + x;
    if (leftMotor > 255)
      leftMotor = 255;
    if (leftMotor < 0)
      leftMotor = 0;

    rightMotor = (127 - y) - x;
    if (rightMotor > 255)
      rightMotor = 255;
    if (rightMotor < 0);
    rightMotor = 0;
  }
  // left motor forwards, right motor backwards
  else if (x > 128) {
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, HIGH);
    digitalWrite(MOTOR_IN4, LOW);

    leftMotor = (x - 128) * 2;
    rightMotor = (x - 128) * 2;
  }
  // left motor backwards, right motor forwards
  else if (x < 128) {
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, HIGH);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, HIGH);

    leftMotor = (128 - x) * 2;
    rightMotor = (128 - x) * 2;
  }
  // stop
  else {
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, LOW);

    leftMotor = 0;
    rightMotor = 0;
  }

  Serial.print("leftMotor: ");
  Serial.print(leftMotor);
  Serial.print(", rightMotor: ");
  Serial.println(rightMotor);
  Serial.end();
  analogWrite(MOTOR_ENB, leftMotor);
  analogWrite(MOTOR_ENA, rightMotor);
}


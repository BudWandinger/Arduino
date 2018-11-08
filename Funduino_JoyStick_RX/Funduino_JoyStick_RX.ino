#include "RF_COMM.h"

#define INCREASE 1
#define DECREASE 0
#define MOTOR_IN1 A2
#define MOTOR_IN2 A3
#define MOTOR_IN3 A4
#define MOTOR_IN4 A5
#define MOTOR_ENA 5
#define MOTOR_ENB 6

unsigned char adr[5] = {0x34, 0x43, 0x10, 0x10, 0x01};
RF_COMM rf(5, adr, 10);

void setup() {
  Serial.begin(9600);
  rf.initialize(UNO, 8, 9);
  rf.RX_Mode();

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);
  pinMode(MOTOR_ENB, OUTPUT);
}

void loop() {
  rf.RX();
  if (rf.get_RX_BUFFER() != NULL) {
    int Xarray[4];
    int Yarray[4];
    int A;
    int B;
    int C;
    int D;
    int E;
    int F;
    int K;

    for (int i = 0; i < 8; i++) {
      if (i < 4)
        Xarray[i] = rf.get_RX_BUFFER(i);
      else if (i >= 4)
        Yarray[i - 4] = rf.get_RX_BUFFER(i);
    }
    A = (rf.get_RX_BUFFER(8) & 0x1) >> 0;
    B = (rf.get_RX_BUFFER(8) & 0x2) >> 1;
    C = (rf.get_RX_BUFFER(8) & 0x4) >> 2;
    D = (rf.get_RX_BUFFER(8) & 0x8) >> 3;
    E = (rf.get_RX_BUFFER(9) & 0x1) >> 0;
    F = (rf.get_RX_BUFFER(9) & 0x2) >> 1;
    K = (rf.get_RX_BUFFER(9) & 0x4) >> 2;
    rf.clear_RX_BUFFER();

    int X = Xarray[0] + Xarray[1] * 10 + Xarray[2] * 100 + Xarray[3] * 1000;
    int Y = Yarray[0] + Yarray[1] * 10 + Yarray[2] * 100 + Yarray[3] * 1000;
    /*
        Serial.print("X: ");
        Serial.println(X);
        Serial.print("Y: ");
        Serial.println(Y);
        Serial.print("A: ");
        Serial.println(A);
        Serial.print("B: ");
        Serial.println(B);
        Serial.print("C: ");
        Serial.println(C);
        Serial.print("D: ");
        Serial.println(D);
        Serial.print("E: ");
        Serial.println(E);
        Serial.print("F: ");
        Serial.println(F);
        Serial.print("K: ");
        Serial.println(K);
        Serial.println();
    */
    X = X + 10 / 2;
    X -= X % 10;
    Y = Y + 10 / 2;
    Y -= Y % 10 / 2;
    X = X / 10;
    Y = Y / 10;
    X = X - 50;
    Y = Y - 50;
    X -= 2;
    Y -= 2;

    if (X < -50)
      X = -50;
    if (Y < -50)
      Y = -50;

    int right = 0;
    int left = 0;

    if (Y > 0) {
      digitalWrite(MOTOR_IN1, HIGH);
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, HIGH);

      analogWrite(MOTOR_ENA, Y * 5);
      analogWrite(MOTOR_ENB, Y * 5);

    }
    else if (Y < 0) {
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, HIGH);
      digitalWrite(MOTOR_IN3, HIGH);
      digitalWrite(MOTOR_IN4, LOW);

      analogWrite(MOTOR_ENA, Y * 5 * -1);
      analogWrite(MOTOR_ENB, Y * 5 * -1);
    }
    else if (X > 0) {
      digitalWrite(MOTOR_IN1, HIGH);
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, HIGH);
      digitalWrite(MOTOR_IN4, LOW);
      analogWrite(MOTOR_ENA, X * 5);
      analogWrite(MOTOR_ENB, X * 5);
    }
    else if (X < 0) {
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, HIGH);
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, HIGH);
      analogWrite(MOTOR_ENA, X * 5 * -1);
      analogWrite(MOTOR_ENB, X * 5 * -1);
    }
    else {
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      analogWrite(MOTOR_ENA, 0);
      analogWrite(MOTOR_ENB, 0);
    }

    //Serial.print("RIGHT: ");
    //Serial.println(right);
    //Serial.print("LEFT: ");
    //Serial.println(left);
    //delay(1000);
  }
}

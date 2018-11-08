#include "RF_COMM.h"

#define A 2
#define B 3
#define C 4
#define D 5
#define E 6
#define F 7
#define K 8
#define X A0
#define Y A1

unsigned char adr[5] = {0x34, 0x43, 0x10, 0x10, 0x01};
RF_COMM rf(5, adr, 10);
// RF_COMM rf;

void setup() {
  Serial.begin(9600);
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(C, INPUT);
  pinMode(D, INPUT);
  pinMode(E, INPUT);
  pinMode(F, INPUT);
  pinMode(K, INPUT);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);
  rf.initialize(UNO, 8, 9);
  rf.TX_Mode();
}

void loop() {
  int controlYint = analogRead(Y);
  int controlXint = analogRead(X);
  int controlA = digitalRead(A);
  int controlB = digitalRead(B);
  int controlC = digitalRead(C);
  int controlD = digitalRead(D);
  int controlE = digitalRead(E);
  int controlF = digitalRead(F);
  int controlK = digitalRead(K);

  unsigned char controlYchar[4] = {};
  unsigned char controlXchar[4] = {};
  unsigned char AtoD = 0x0;
  unsigned char EtoK = 0x0;

  for (int i = 0; i < 4; i++) {
    controlYchar[i] = controlYint % 10;
    controlYint /= 10;
    controlXchar[i] = controlXint % 10;
    controlXint /= 10;
  }
  AtoD |= (controlA << 0);
  AtoD |= (controlB << 1);
  AtoD |= (controlC << 2);
  AtoD |= (controlD << 3);
  EtoK |= (controlE << 0);
  EtoK |= (controlF << 1);
  EtoK |= (controlK << 2);
  EtoK |= (0 << 3);

  unsigned char control[10] = {};
  for (int i = 0; i < 8; i++) {
    if (i < 4)
      control[i] = controlXchar[i];
    else if (i >= 4)
      control[i] = controlYchar[i - 4];
  }
  control[8] = AtoD;
  control[9] = EtoK;
/*
  Serial.print("control: ");
  Serial.print(+control[9], BIN);
  Serial.print(" -  ");
  Serial.print(+control[8], BIN);
  Serial.print(" -  ");
  for (int i = 7; i >= 0; i--) {
    Serial.print(+control[i]);
    Serial.print(" ");
    if (i == 4)
      Serial.print(" -  ");
  }
  Serial.println();
*/
  rf.set_TX_BUFFER(control);
  rf.TX();
  rf.clear_TX_BUFFER();
}
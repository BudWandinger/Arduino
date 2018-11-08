#include <Servo.h>

//Servo Defines
/*********************************/
#define PAN_SERVO 5
#define TILT_SERVO 6

#define panMax 180
#define panRight 190
#define panForward 90
#define panLeft 0
#define panMin 0

#define tiltMax 155
#define tiltForward 23
#define tiltUp 112
#define tiltMin 0
/*********************************/

//Funduio_Joystick Defines
/*********************************/
#define A 24
#define B 25
#define C 26
#define D 27
#define E 28
#define F 29
#define K 30
#define X A1
#define Y A2
/*********************************/

Servo panServo;
Servo tiltServo;

void setup() {
  pinMode(PAN_SERVO, OUTPUT);
  pinMode(TILT_SERVO, OUTPUT);

  panServo.attach(PAN_SERVO);
  tiltServo.attach(TILT_SERVO);

  panServo.write(panForward);
  tiltServo.write(tiltForward);

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
  unsigned char *con;
  con = readCon();

  if (*(con + 9) & 0x4) {
    aim(panForward, tiltForward);
  }
  else if (*(con + 8) & 0x1) {
    aim(panServo.read(), tiltUp);
  }
  else if (*(con + 8) & 0x2) {
    aim(panRight, tiltServo.read());
  }
  else if ( *(con + 8) & 0x4) {
    aim(panServo.read(), tiltMin);
  }
  else if ( *(con + 8) & 0x8) {
    aim(panLeft, tiltServo.read());
  }
  else if ( *(con + 9) & 0x1) {
    aim(panForward, tiltServo.read());
  }
  else if ( *(con + 9) & 0x2) {
    aim(panServo.read(), tiltForward);
  }
  else {
    writeJoy(con);
  }
  delete[] con;
}

void writeJoy(unsigned char * con)
{
  int x = charToInt(con);
  int y = charToInt(con + 4);

  if (x < 510)
    aimPan(panServo.read() - 1);
  else if (x > 520)
    aimPan(panServo.read() + 1);

  if (y < 510)
    aimTilt(tiltServo.read() - 1);
  else if (y > 520)
    aimTilt(tiltServo.read() + 1);

  if ((y < 510 && y > 400) || (y > 520 && y < 600) || (x < 510 && x > 400) || (x > 520 && x < 600))
    delay(30);
  else if ((y <= 400 && y > 300) || (y >= 600 && y < 700) || (x <= 400 && x > 300) || (x >= 600 && x < 700))
    delay(20);
  else if ((y <= 300 && y > 200) || (y >= 700 && y < 800) || (x <= 300 && x > 200) || (x >=700 && x < 800))
    delay(10);
  else if ((y <= 200 && y > 100) || (y >= 800 && y < 900) || (x <= 200 && x > 100) || (x >= 800 && x < 900))
    delay(5);
}

int charToInt(unsigned char *oldChar)
{
  int newInt = 0;
  for (int i = 0; i < 4; i++) {
    newInt += *(oldChar + i) * pow(10, i);
  }
  return newInt;
}

void aimPan(int pan)
{
  if (pan < panMin)
    panServo.write(panMin);
  else if (pan > panMax)
    panServo.write(panMax);
  else
    panServo.write(pan);
}
void aimTilt(int tilt)
{
  if (tilt < tiltMin)
    tiltServo.write(tiltMin);
  else if (tilt > tiltMax)
    tiltServo.write(tiltMax);
  else
    tiltServo.write(tilt);
}
void aim(int pan, int tilt)
{
  aimPan(pan);
  aimTilt(tilt);
}

unsigned char * readCon(void)
{
  int conYint = analogRead(Y);
  int conXint = analogRead(X);
  int conA = not(digitalRead(A));
  int conB = not(digitalRead(B));
  int conC = not(digitalRead(C));
  int conD = not(digitalRead(D));
  int conE = not(digitalRead(E));
  int conF = not(digitalRead(F));
  int conK = not(digitalRead(K));

  unsigned char conYchar[4] = {};
  unsigned char conXchar[4] = {};
  unsigned char conAtoD = 0x0;
  unsigned char conEtoK = 0x0;

  for (int i = 0; i < 4; i++) {
    conYchar[i] = conYint % 10;
    conYint /= 10;
    conXchar[i] = conXint % 10;
    conXint /= 10;
  }
  conAtoD |= (conA << 0);
  conAtoD |= (conB << 1);
  conAtoD |= (conC << 2);
  conAtoD |= (conD << 3);
  conEtoK |= (conE << 0);
  conEtoK |= (conF << 1);
  conEtoK |= (conK << 2);
  conEtoK |= (0 << 3);

  unsigned char *con = new unsigned char[10];
  for (int i = 0; i < 8; i++) {
    if (i < 4)
      *(con + i) = conXchar[i];
    else if (i >= 4)
      *(con + i) = conYchar[i - 4];
  }
  *(con + 8) = conAtoD;
  *(con + 9) = conEtoK;

  /*
    Serial.print(*(con+9), BIN);
    Serial.print(" -  ");
    Serial.print(*(con+8), BIN);
    Serial.print(" -  ");
    for (int i = 7; i >= 0; i--) {
    Serial.print(*(con+i));
    Serial.print(" ");
    if (i == 4)
      Serial.print(" -  ");
    }
    Serial.println();
    delay(100);
  */

  return con;
}


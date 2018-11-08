#include "PS2_CONTROL.h"

PS2_CONTROL con;

void setup()
{
  con.initializeController();
  Serial.begin(9600);
}

void loop()
{
  
}
































/*#include <SPI.h>
#define SS 10

const int BUFFER_SIZE = 9;
const int DELAY = 6;

// Command constants
const byte READ[BUFFER_SIZE] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const byte READ_VIBRATE[BUFFER_SIZE] = {0x01, 0x42, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00};
const byte ENTER_CONFIG[BUFFER_SIZE] = {0x01, 0x43, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
const byte EXIT_CONFIG[BUFFER_SIZE]  = {0x01, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const byte SET_MODE_DIGITAL_LOCK[BUFFER_SIZE]  = {0x01, 0x44, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00};
const byte SET_MODE_ANALOG_LOCK[BUFFER_SIZE] = {0x01, 0x44, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00};
const byte QUERY_MODEL_MODE[BUFFER_SIZE] = {0x01, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const byte VIBRATE_ENABLE[BUFFER_SIZE] = {0x01, 0x4D, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF};
const byte VIBRATE_DISABLE[BUFFER_SIZE] = {0x01, 0x4D, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// These are read from the 4th byte of controller data
const byte SELECT_MASK = 0x01;
const byte LEFT_JOY_BUTTON = 0x02;
const byte RIGHT_JOY_BUTTON = 0x04;
const byte START_MASK = 0x08;
const byte DPAD_UP = 0x10;
const byte DPAD_RIGHT = 0x20;
const byte DPAD_DOWN = 0x40;
const byte DPAD_LEFT = 0x80;

// These are read from the 5th byte of controller data
const byte L2_MASK = 0x01;
const byte R2_MASK = 0x02;
const byte L1_MASK = 0x04;
const byte R1_MASK = 0x08;
const byte TRIANGLE_MASK = 0x10;
const byte CIRCLE_MASK = 0x20;
const byte CROSS_MASK = 0x40;
const byte SQUARE_MASK = 0x80;


void setup() {
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  SPI.begin();
  byte *buffer = new byte[BUFFER_SIZE];
  delay(1000);
  enterConfig(buffer);
  printBuffer(buffer);
  delay(1000);
  setModeAnalogLock(buffer);
  printBuffer(buffer);
  delay(1000);
  exitConfig(buffer);
  printBuffer(buffer);
  delay(1000);
  vibrateEnable(buffer);
  printBuffer(buffer);
  delay(1000);
  delete[] buffer;
}

void loop() {
  byte *buffer = new byte[BUFFER_SIZE];
  //queryModelMode(buffer);
  readControllerVibrate(buffer);
  printBuffer(buffer);
  delete[] buffer;
}

void readWrite(byte *buffer, byte *command)
{
  //printBuffer(command);
  SPI.beginTransaction(SPISettings(57600, LSBFIRST, SPI_MODE2));
  digitalWrite(SS, LOW);
  for (int i = 0; i < BUFFER_SIZE; i++) {
    *(buffer + i) = SPI.transfer(*(command + i));
    delayMicroseconds(DELAY);
  }
  digitalWrite(SS, HIGH);
  SPI.endTransaction();
}

void readController(byte *buffer)
{
  readWrite(buffer, READ);
}
void readControllerVibrate(byte *buffer)
{
  readWrite(buffer, READ_VIBRATE);
}

void enterConfig(byte *buffer)
{
  readWrite(buffer, ENTER_CONFIG);
}

void exitConfig(byte *buffer)
{
  readWrite(buffer, EXIT_CONFIG);
}

void setModeDigitalLock( byte *buffer)
{
  readWrite(buffer, SET_MODE_DIGITAL_LOCK);
}

void setModeAnalogLock(byte *buffer)
{
  readWrite(buffer, SET_MODE_ANALOG_LOCK);
}

void queryModelMode(byte *buffer)
{
  readWrite(buffer, QUERY_MODEL_MODE);
}

void vibrateEnable(byte *buffer)
{
  readWrite(buffer, VIBRATE_ENABLE);
}

void vibrateDisable(byte *buffer)
{
  readWrite(buffer, VIBRATE_DISABLE);
}

void printBuffer(byte * buffer)
{
  Serial.begin(9600);
  Serial.print("Buffer: ");
  for (int i = 0; i < BUFFER_SIZE; i++) {
    Serial.print(+ * (buffer + i), HEX);
    Serial.print(" ");
  }
  Serial.println();
  Serial.end();
}
*/

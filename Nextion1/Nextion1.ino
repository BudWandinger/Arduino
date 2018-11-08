#include "Nextion.h"

void setup() {
  Serial.begin(9600);
  nexInit();
  unsigned char cmd0[] = "vis p0,0";
  sendCommand(cmd0);
  unsigned char cmd1[] = "vis g0,0";
  sendCommand(cmd1);
  dbSerialPrintln("setup done");
  delay(1000);

}

void intToCharArray(unsigned char* numberArray, int numberInt)
{
  *(numberArray + 4) = '\0';

  *(numberArray + 3) = '0' + (numberInt % 10);
  numberInt /= 10;
  *(numberArray + 2) = '0' + (numberInt % 10);
  numberInt /= 10;
  *(numberArray + 1) = '0' + (numberInt % 10);
  numberInt /= 10;
  *(numberArray + 0) = '0' + (numberInt % 10);
}

void addToInstruction(unsigned char *instruction, unsigned char *add)
{
  int cursor = 0;
  while (*(instruction + cursor) != '\0')
    cursor++;

  for (int i = 0; * (add + i) != '\0'; i++, cursor++)
    *(instruction + cursor) = *(add + i);
  *(instruction + cursor) = '\0';
}

void sendInstruction(unsigned char *command, int Xstart, int Ystart,
                     int Xwidth, int Ywidth, unsigned char *color)
{
  unsigned char space[] = " ";
  unsigned char comma[] = ",";

  unsigned char *XstartArray = new unsigned char[5];
  unsigned char *YstartArray = new unsigned char[5];
  unsigned char *XwidthArray = new unsigned char[5];
  unsigned char *YwidthArray = new unsigned char[5];

  intToCharArray(XstartArray, Xstart);
  intToCharArray(YstartArray, Ystart);
  intToCharArray(XwidthArray, Xwidth);
  intToCharArray(YwidthArray, Ywidth);

  int instructionSize = sizeOf(command) + 16 + sizeOf(color)
                        + sizeOf(space) + sizeOf(comma) * 4 + 1;

  unsigned char *instruction = new unsigned char[instructionSize];
  *(instruction + 0) = '\0';
  int instructionCursor = 0;

  addToInstruction(instruction, command);
  addToInstruction(instruction, space);
  addToInstruction(instruction, XstartArray);
  addToInstruction(instruction, comma);
  addToInstruction(instruction, YstartArray);
  addToInstruction(instruction, comma);
  addToInstruction(instruction, XwidthArray);
  addToInstruction(instruction, comma);
  addToInstruction(instruction, YwidthArray);
  addToInstruction(instruction, comma);
  addToInstruction(instruction, color);

  sendCommand(instruction);

  delete[] instruction;
  delete[] XstartArray;
  delete[] YstartArray;
  delete[] XwidthArray;
  delete[] YwidthArray;
}

int sizeOf(unsigned char *string)
{
  int size = 0;
  for (int i = 0; string[i] != '\0'; i++)
    size++;
  return size;
}

void loop() {

  unsigned char *cmd1 = "fill 0,0,240,400,BLACK";
  sendCommand(cmd1);
  delay(500);

  unsigned char *color = "YELLOW";

  //  sendInstruction("fill", 0, 0, 100, 50, "YELLOW");

  for (int w = 0; w < 240; w += 20) {
    for (int h = 0; h < 400; h += 20) {
      sendInstruction("fill", w, h, 20, 20, "YELLOW");
    }
  }

  for (int i = 0; i < 10; i++) {
    sendInstruction("fill", 0, 0, 240, 400, "WHITE");
    delay(200);
    sendInstruction("fill", 0, 0, 240, 400, "BLACK");
    delay(200);
  }
  delay(500);

  Serial.flush();
}


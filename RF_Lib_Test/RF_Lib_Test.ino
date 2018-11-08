#include "RF_COMM.h"

unsigned char adr[5] = {0x34, 0x43, 0x10, 0x10, 0x01};
RF_COMM rf(5, adr, 4);

void setup() {
  Serial.begin(9600);
  rf.initialize(UNO, 8, 9);
  rf.RX_Mode();
}

void loop() {
  rf.RX();
  if (rf.get_RX_BUFFER() != NULL) {
    unsigned char controlYchar[2];
    unsigned char controlXchar[2];

    controlYchar[0] = *(rf.get_RX_BUFFER() + 0);
    controlYchar[1] = *(rf.get_RX_BUFFER() + 1);
    controlXchar[0] = *(rf.get_RX_BUFFER() + 2);
    controlXchar[1] = *(rf.get_RX_BUFFER() + 3);

    int controlYint = 0;
    int controlXint = 0;
    controlYint = controlYchar[0] & 0xF
    
    
    for (int i = 0; i < rf.get_TX_PLOAD_WIDTH(); i++) {
      Serial.print("RX_BUFFER: ");
      Serial.print(*(rf.get_RX_BUFFER() + i), HEX);
      Serial.print(" ");
    }
    Serial.println();
    rf.clear_RX_BUFFER();
  }
}


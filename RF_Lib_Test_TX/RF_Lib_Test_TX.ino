#include<SPI.h>
#include "RF_COMM.h"

unsigned char adr[5] = {0x34, 0x43, 0x10, 0x10, 0x01};
RF_COMM rf(5, adr, 1);

void setup() {
  rf.initialize(3, 4, 5);
  SPI.begin();
  rf.TX_Mode();
}

void loop() {
  for (unsigned char i = 0; i < 32; i++) {
    Serial.begin(9600);
    rf.set_TX_BUFFER(&i);
    //Serial.print("tx_buf: ");
    for (int i = 0; i < rf.get_TX_PLOAD_WIDTH(); i++) {
      Serial.print(rf.get_TX_BUFFER(i), HEX);
    }
    Serial.println();
    rf.TX();
    rf.clear_TX_BUFFER();
    Serial.end();
    delay(100);
    
  }
}

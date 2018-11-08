#include <SPI.h>
#include "RF_COMM.h"
#include "PS2_CONTROL.h"

unsigned char adr[5] = {0x34, 0x43, 0x10, 0x10, 0x01};
RF_COMM rf(5, adr, 9);

PS2_CONTROL con(2);

void setup() {
  rf.initialize(3, 4, 5);
  SPI.begin();
  rf.TX_Mode();
  con.initializeController();
}

void loop() {
  con.readController();
  con.printBuffer();
  rf.set_TX_BUFFER(con.getBuffer());
  rf.TX();
  rf.clear_TX_BUFFER();
  con.clearBuffer();

}

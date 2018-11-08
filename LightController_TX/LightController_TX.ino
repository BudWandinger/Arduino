/*
   LightController_TX.ino
   By Bud Wandinger
   Last Modified on May 5, 2017

   The purpose of this sketch is utilize the nRF24L01 module
   for wireless command protocol. This sketch works as the
   transmitter module which relays simple hex commands based
   basic button input from the user.
*/

#include "RF.h"

#define BUTTON_1 22
#define BUTTON_2 23

// Define RF address for communication
unsigned char TX_ADDRESS[TX_ADR_WIDTH] =
{
  0x34, 0x43, 0x10, 0x10, 0x01
};

unsigned char rx_buf[TX_PLOAD_WIDTH] = {0};
unsigned char tx_buf[TX_PLOAD_WIDTH] = {0};


void setup() {
  Serial.begin(9600);
  pinMode(CE, OUTPUT);
  pinMode(CSN, OUTPUT);
  pinMode(IRQ, INPUT);
  SPI.begin();
  delay(50);
  init_io();
  unsigned char sstatus = SPI_Read(STATUS);
  Serial.println("*******************TX_Mode Start****************************");
  Serial.print("status = ");
  Serial.println(sstatus, HEX);
  TX_Mode(TX_ADDRESS, tx_buf);

  // BUTTONS
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
}

void loop() {
  if (digitalRead(BUTTON_1) == HIGH && digitalRead(BUTTON_2) == HIGH) {
    tx_buf[0] = 0x01;
  }
  else if (digitalRead(BUTTON_1) == HIGH && digitalRead(BUTTON_2) == LOW) {
    tx_buf[0] = 0x02;
  }
  else if (digitalRead(BUTTON_1) == LOW && digitalRead(BUTTON_2) == HIGH) {
    tx_buf[0] = 0x03;
  }
  else{
    tx_buf[0] = 0x00;
  }

  unsigned char sstatus = SPI_Read(STATUS);
  if (sstatus & TX_DS) {
    SPI_RW_Reg(FLUSH_TX, 0);
    SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);
  }
  if (sstatus & MAX_RT) {
    SPI_RW_Reg(FLUSH_TX, 0);
    SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);
  }
  SPI_RW_Reg(WRITE_REG + STATUS, sstatus);
  delay(15);
}


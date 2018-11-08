/*
   LightController_RX.ino
   By Bud Wandinger
   Last Modified on May 5, 2017

   The purpose of this sketch is to utilize the nRF24L01 module
   for wireless command protocal. This sketch works as the reciever
   module which recieved simple kex commands, decodes the commands,
   and then controls a set of LEDs coinciding with the recieved
   comands.
*/

#include "RF.h"

#define LED_1 7
#define LED_2 4
#define LED_3 2

unsigned char TX_ADDRESS[TX_ADR_WIDTH]  =
{
  0x34, 0x43, 0x10, 0x10, 0x01
}; // Define a static TX address

unsigned char rx_buf[TX_PLOAD_WIDTH] = {0}; // initialize value
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
  Serial.println("*******************RX_Mode Start****************************");
  Serial.print("status = ");
  Serial.println(sstatus, HEX);    // default value should be ‘E’
  RX_Mode(TX_ADDRESS);

  //LEDs
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
}

void loop() {
  unsigned char sstatus = SPI_Read(STATUS);
  Serial.print(sstatus);
  if (sstatus & RX_DR) {
    Serial.println("in IF");
    SPI_Read_Buf(RD_RX_PLOAD, rx_buf, TX_PLOAD_WIDTH);
    SPI_RW_Reg(FLUSH_RX, 0);
    if (rx_buf[0] == 0x01) {
      Serial.println("Recieved 0x01");
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, LOW);
      digitalWrite(LED_3, LOW);
    }
    else if (rx_buf[0] == 0x02) {
      Serial.println("Recieved 0x02");
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, HIGH);
      digitalWrite(LED_3, LOW);
    }
    else if (rx_buf[0] == 0x03) {
      Serial.println("Recieved 0x03");
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, LOW);
      digitalWrite(LED_3, HIGH);
    }
    else if (rx_buf[0] == 0x0){
      Serial.println("Recieved 0x00");
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, LOW);
      digitalWrite(LED_3, LOW);
    }
  }
  SPI_RW_Reg(WRITE_REG + STATUS, sstatus);
  delay(15);
}






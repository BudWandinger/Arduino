/*
 * RF_COMM.cpp - Library for using nRF24L01 modules
 * Created by Bud Wandinger
 * Last Modified May 17, 2017 *** HAPPY BIRTHDAY TO THE CREATOR!!!
 */

#include "RF_COMM.h"

//**************************************************
// RF_COMM DEFAULT constructor;
// Description:
// Constructs the RF_COMM class with default values:
// - TX_ADDRESS_WIDTH = 5
// - TX_ADDRESS[] = {0x34, 0x43, 0x10, 0x10, 0x01}
// - TX+PLOAD_WIDTH = 1;
// -rx_buf and tx_buf = NULL
//**************************************************
RF_COMM::RF_COMM(void)
{
  TX_ADR_WIDTH = 5;
  
  TX_ADDRESS = new unsigned char [TX_ADR_WIDTH];
  
  TX_ADDRESS[0] = 0x34;
  TX_ADDRESS[1] = 0x43;
  TX_ADDRESS[2] = 0x10;
  TX_ADDRESS[3] = 0x10;
  TX_ADDRESS[4] = 0x01;
  
  TX_PLOAD_WIDTH = 1;

  rx_buf = NULL;
  tx_buf = NULL;
}

//**************************************************
// RF_COMM constructor;
// Description:
// Constructs the RF_COMM class. Requires:
// - TX address width
// - a pointer to TX address
// - TX payload width
//**************************************************
RF_COMM::RF_COMM(int TX_ADR_WIDTH, unsigned char *TX_ADDRESS, int TX_PLOAD_WIDTH)
{
  this->TX_ADR_WIDTH = TX_ADR_WIDTH;
  this->TX_ADDRESS = new unsigned char[TX_ADR_WIDTH];
  for (int i = 0; i < TX_ADR_WIDTH; i++)
    *(this->TX_ADDRESS + i) = *(TX_ADDRESS + i);
  this->TX_PLOAD_WIDTH = TX_PLOAD_WIDTH;
  rx_buf = NULL;
  tx_buf = NULL;
}

//**************************************************
// Function: TX();
// Description:
// Transmitts a data packet the size of TX_PLOAD_WIDTH
//**************************************************
int RF_COMM::TX(void)
{
  if(MODE == RX_MODE)
    return 0;
  unsigned char sstatus = SPI_Read(STATUS);
  if (sstatus & TX_DS || sstatus & MAX_RT) {
    SPI_RW_Reg(FLUSH_TX, 0);
    SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);
  }
  SPI_RW_Reg(WRITE_REG + STATUS, sstatus);
  //delay(DELAY);
  return 1;
}

//**************************************************
// Function: set_TX_BUFFER();
// Description:
// fills the tx_buf
//**************************************************
void RF_COMM::set_TX_BUFFER(unsigned char *tx_buf)
{
  if(this->tx_buf != NULL)
    clear_TX_BUFFER();
  
  this->tx_buf = new unsigned char[TX_PLOAD_WIDTH];
  for(int i = 0; i < TX_PLOAD_WIDTH; i++)
    *(this->tx_buf + i) = *(tx_buf + i);
}


//**************************************************
// Function: clear_TX_BUFFER();
// Description:
// deallocates memory for tx_buf and sets 
// tx_buf = NULL
//**************************************************
void RF_COMM::clear_TX_BUFFER(void)
{
  delete[] tx_buf;
  tx_buf = NULL;
}


//**************************************************
// Function: get_TX_BUFFER();
// Description:
// returns an unsigned char * of tx_buf
//**************************************************

unsigned char * RF_COMM::get_TX_BUFFER(void)
{
  return tx_buf;
}
unsigned char RF_COMM::get_TX_BUFFER(int index)
{
  return *(tx_buf + index);
}

//**************************************************
// Function: RX();
// Description:
// Recieves a data packet the size of TX_PLOAD_WIDTH
//**************************************************
int RF_COMM::RX(void)
{
  if(MODE == TX_MODE)
    return 0;
  unsigned char sstatus = SPI_Read(STATUS);
  if (sstatus & RX_DR) {
    rx_buf = new unsigned char[TX_PLOAD_WIDTH];
    SPI_Read_Buf(RD_RX_PLOAD, rx_buf, TX_PLOAD_WIDTH);
    SPI_RW_Reg(FLUSH_RX, 0);
  }
  SPI_RW_Reg(WRITE_REG + STATUS, sstatus);
  delay(DELAY);
  return 1;
}

//**************************************************
// Function: clear_RX_BUFFER();
// Description:
// deallocates memory for rx_buf and sets 
// rx_buf = NULL
//**************************************************

void RF_COMM::clear_RX_BUFFER(void)
{
  delete[] rx_buf;
  rx_buf = NULL;
}


//**************************************************
// Function: get_RX_BUFFER();
// Description:
// returns an unsigned char * of rx_buf
//**************************************************
unsigned char * RF_COMM::get_RX_BUFFER(void)
{
  return rx_buf;
}

unsigned char RF_COMM::get_RX_BUFFER(int index)
{
	return *(rx_buf + index);
}

//**************************************************
// Function: get_TX_ADR_WIDTH();
// Description:
// returns TX_ADR_WIDTH as an int
//**************************************************
int RF_COMM::get_TX_ADR_WIDTH(void)
{
  return this->TX_ADR_WIDTH;
}

//**************************************************
// Function: set_TX_ADR_WIDTH();
// Description:
// sets TX_ADR_WIDTH from an int
//**************************************************
void RF_COMM::set_TX_ADR_WIDTH(int TX_ADR_WIDTH)
{
  this->TX_ADR_WIDTH = TX_ADR_WIDTH;
}


//**************************************************
// Function: get_TX_PLOAD_WIDTH();
// Description:
// returns the TX payload width as an int
//**************************************************
int RF_COMM::get_TX_PLOAD_WIDTH(void)
{
  return this->TX_PLOAD_WIDTH;
}


//**************************************************
// Function: set_TX_PLOAD_WIDTH();
// Description:
// sets the TX payload width from an int
//**************************************************
void RF_COMM::set_TX_PLOAD_WIDTH(int TX_PLOAD_WIDTH)
{
  this->TX_PLOAD_WIDTH = TX_PLOAD_WIDTH;
}

//**************************************************
// Function: get_TX_ADDRESS();
// Description:
// returns a pointer to a static array TX_ADDRESS
//**************************************************
static unsigned char * RF_COMM::get_TX_ADDRESS(void)
{
  return this->TX_ADDRESS;
}

//**************************************************
// Function: set_TX_ADDRESS();
// Description:
// sets TX_ADDRESS from a pointer to unsigned char,
// and an int of new address width
//**************************************************
void RF_COMM::set_TX_ADDRESS(unsigned char *TX_ADDRESS, int TX_ADR_WIDTH)
{
  delete[]this->TX_ADDRESS;
  set_TX_ADR_WIDTH(TX_ADR_WIDTH);
  *this->TX_ADDRESS = new unsigned char[TX_ADR_WIDTH];
  for (int i = 0; i < TX_ADR_WIDTH; i++)
    this->TX_ADDRESS[i] = TX_ADDRESS[i];
}

//**************************************************
// Function: initialize();
// Description:
// flash led one time,chip enable(ready to TX or RX Mode),
// Spi disable,Spi clock line init high
//**************************************************
void RF_COMM::initialize(int CSN, int CE, int IRQ)
{
  Serial.begin(9600);
  Serial.println("Initializing SPI protocol for nRF24L01...");

  this->CSN = CSN;
  this->CE = CE;
  this->IRQ = IRQ;
  pinMode(CE, OUTPUT);
  pinMode(CSN, OUTPUT);
  pinMode(IRQ, INPUT);

  digitalWrite(IRQ, 0);
  digitalWrite(CE, 0);      // chip enable
  digitalWrite(CSN, 1);     // Spi disable

  Serial.println("Initialization completed with the following settings:");
  Serial.println("Micro Controller: Arduino ");
/*  if (MCtype == UNO)
    Serial.println("UNO");
  else if (MCtype == MEGA)
    Serial.println("MEGA");

  Serial.print("Tranciever Module Mode: ");
  if(MODE == RX_MODE)
    Serial.println("RX_MODE");
  else if(MODE == TX_MODE)
    Serial.println("TX_MODE");
*/  Serial.print("CSN Pin: ");
  Serial.println(CSN);
  Serial.print("CE Pin: ");
  Serial.println(CE);
  Serial.print("IRQ Pin: ");
  Serial.println(IRQ);
  Serial.end();
}

//**************************************************
// Function: ger_MODE();
// Description:
// returns the current MODE of the tranciever module
// either:
// - RX_MODE: 0
// - TX_MODE: 1
//**************************************************
int RF_COMM::get_MODE(void)
{
  return MODE;
}

/************************************************************************
**   * Function: SPI_RW();

   Description:
   Writes one unsigned char to nRF24L01, and return the unsigned char read
   from nRF24L01 during write, according to SPI protocol
************************************************************************/
unsigned char RF_COMM::SPI_RW(unsigned char Byte)
{
  unsigned char slaveIn;
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  slaveIn = SPI.transfer(Byte);
  SPI.endTransaction();
  return slaveIn;
}

/**************************************************
   Function: SPI_RW_Reg();

   Description:
   Writes value 'value' to register 'reg'
  /**************************************************/
unsigned char RF_COMM::SPI_RW_Reg(unsigned char reg, unsigned char value)
{
  unsigned char status;

  digitalWrite(CSN, 0);                   // CSN low, init SPI transaction
  SPI_RW(reg);                            // select register
  SPI_RW(value);                          // ..and write value to it..
  digitalWrite(CSN, 1);                   // CSN high again

  return (status);                  // return nRF24L01 status unsigned char
}

/**************************************************
   Function: SPI_Read();

   Description:
   Read one unsigned char from nRF24L01 register, 'reg'
  /**************************************************/
unsigned char RF_COMM::SPI_Read(unsigned char reg)
{
  unsigned char reg_val;

  digitalWrite(CSN, 0);                // CSN low, initialize SPI communication...
  SPI_RW(reg); 						   // Select register to read from..
  reg_val = SPI_RW(0);                 // ..then read register value
  digitalWrite(CSN, 1);                // CSN high, terminate SPI communication

  return (reg_val);                    // return register value
}

/**************************************************
   Function: SPI_Read_Buf();

   Description:
   Reads 'unsigned chars' #of unsigned chars from register 'reg'
   Typically used to read RX payload, Rx/Tx address
  /**************************************************/
unsigned char RF_COMM::SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
  unsigned char sstatus, i;

  digitalWrite(CSN, 0);                   // Set CSN low, init SPI tranaction
  sstatus = SPI_RW(reg);             // Select register to write to and read status unsigned char

  for (i = 0; i < bytes; i++)
  {
    pBuf[i] = SPI_RW(0);    // Perform SPI_RW to read unsigned char from nRF24L01
  }

  digitalWrite(CSN, 1);                   // Set CSN high again

  return (sstatus);                 // return nRF24L01 status unsigned char
}

/**************************************************
   Function: SPI_Write_Buf();

   Description:
   Writes contents of buffer '*pBuf' to nRF24L01
   Typically used to write TX payload, Rx/Tx address
  /**************************************************/
unsigned char RF_COMM::SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
  unsigned char sstatus, i;

  digitalWrite(CSN, 0);                   // Set CSN low, init SPI tranaction
  sstatus = SPI_RW(reg);             // Select register to write to and read status unsigned char
  for (i = 0; i < bytes; i++)       // then write all unsigned char in buffer(*pBuf)
  {
    SPI_RW(*pBuf++);
  }
  digitalWrite(CSN, 1);                   // Set CSN high again
  return (sstatus);                 // return nRF24L01 status unsigned char
}

/**************************************************
   Function: TX_Mode();

   Description:
   This function initializes one nRF24L01 device to
   TX mode, set TX address, set RX address for auto.ack,
   fill TX payload, select RF channel, datarate & TX pwr.
   PWR_UP is set, CRC(2 unsigned chars) is enabled, & PRIM:TX.

   ToDo: One high pulse(>10us) on CE will now send this
   packet and expext an acknowledgment from the RX device.
 **************************************************/
void RF_COMM::TX_Mode(void)
{
  unsigned char sstatus = SPI_Read(STATUS);
  Serial.begin(9600);
  Serial.println("*******************TX_Mode Start****************************");
  Serial.print("status = ");
  Serial.println(sstatus, HEX);
  Serial.end();
  
  digitalWrite(CE, 0);

  SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack

  SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
  SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
  SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...
  SPI_RW_Reg(WRITE_REG + RF_CH, 40);        // Select RF channel 40
  SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);     // Set PWR_UP bit, enable CRC(2 unsigned chars) & Prim:TX. MAX_RT & TX_DS enabled..
  SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);

  digitalWrite(CE, 1);
  MODE = TX_MODE;
}


/**************************************************
   Function: RX_Mode();

   Description:
   This function initializes one nRF24L01 device to
   RX mode, set RX address, set TX address for auto.ack,
   fill RX payload, select RF channel, datarate & RX pwr.
   PWR_UP is set, CRC(2 unsigned chars) is enabled, & PRIM:RX.

   ToDo: One high pulse(>10us) on CE will now send this
   packet and expext an acknowledgment from the TX device.
 **************************************************/
void RF_COMM::RX_Mode(void)
{
  unsigned char sstatus = SPI_Read(STATUS);
  Serial.begin(9600);
  Serial.println("*******************RX_Mode Start****************************");
  Serial.print("status = ");
  Serial.println(sstatus, HEX);
  Serial.end();
  
  digitalWrite(CE, 0);

  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // Use the same address on the RX device as the TX device
  SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
  SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
  SPI_RW_Reg(WRITE_REG + RF_CH, 40);        // Select RF channel 40
  SPI_RW_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH); // Select same RX payload width as TX Payload width
  SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);     // Set PWR_UP bit, enable CRC(2 unsigned chars) & Prim:RX. RX_DR enabled..
  digitalWrite(CE, 1);                             // Set CE pin high to enable RX device
  MODE = RX_MODE;
  //  This device is now ready to receive one packet of 16 unsigned chars payload from a TX device sending to address
  //  '3443101001', with auto acknowledgment, retransmit count of 10, RF channel 40 and datarate = 2Mbps.
}

/*
 * RF_COMM.h - Librarie for using nRF24L01 module
 * Created by Bud Wandinger
 * Last Modified May 17, 2017 *** HAPPY BIRTHDAY TO THE CREATOR!!
 */

#ifndef RF_COMM_H
#define RF_COMM_H

#include <Arduino.h>
#include <SPI.h>

class RF_COMM {
  public:
    RF_COMM(void);
    RF_COMM(int TX_ADR_WIDTH, unsigned char *TX_ADDRESS, int TX_PLOAD_WIDTH);
    void initialize(int IRQ, int CE, int CSN);
    int get_MODE(void);
    int RX(void);
	int TX(void);
    void clear_RX_BUFFER(void);
	void clear_TX_BUFFER(void);
    unsigned char * get_RX_BUFFER(void);
	unsigned char * get_TX_BUFFER(void);
	unsigned char get_RX_BUFFER(int index);
	unsigned char get_TX_BUFFER(int index);
    void set_TX_BUFFER(unsigned char *tx_buf);
    void TX_Mode(void);
    void RX_Mode(void);
    int get_TX_ADR_WIDTH(void);
    int get_TX_PLOAD_WIDTH(void);
    void set_TX_PLOAD_WIDTH(int TX_PLOAD_WIDTH);
    unsigned char * get_TX_ADDRESS(void);
    void set_TX_ADDRESS(unsigned char *TX_ADDRESS, int TX_ADR_WIDTH);

  private:
    void set_TX_ADR_WIDTH(int TX_ADR_WIDTH);
    unsigned char SPI_RW(unsigned char Byte);
    unsigned char SPI_RW_Reg(unsigned char reg, unsigned char value);
    unsigned char SPI_Read(unsigned char reg);
    unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes);
    unsigned char SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes);

    int TX_ADR_WIDTH;
    unsigned char *TX_ADDRESS;
    int TX_PLOAD_WIDTH;
    unsigned char *rx_buf;
    unsigned char *tx_buf;

    int IRQ;
    int CE;
    int CSN;

    int MODE;
};

//****************************************************
// SPI(nRF24L01) macros
#define UNO 0
#define MEGA 1
#define DELAY 100
#define RX_MODE 0
#define TX_MODE 1
//****************************************************
// SPI(nRF24L01) commands
#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register
//***************************************************
#define RX_DR    0x40
#define TX_DS    0x20
#define MAX_RT   0x10
//***************************************************
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address

//************************************************
#endif

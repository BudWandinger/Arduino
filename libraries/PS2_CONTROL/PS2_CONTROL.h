#ifndef PS2_CONTROL_H
#define PS2_CONTROL_H

#include <Arduino.h>
#include <SPI.h>

class PS2_CONTROL {
  public:
    PS2_CONTROL(void);
    PS2_CONTROL(int SS);
    ~PS2_CONTROL(void);
    void initializeController(void);

    bool getSelect(void);
    bool getLeftJoyButton(void);
    bool getRightJoyButton(void);
    bool getStart(void);
    bool getDpadUp(void);
    bool getDpadRight(void);
    bool getDpadDown(void);
    bool getDpadLeft(void);
    bool getL2(void);
    bool getR2(void);
    bool getL1(void);
    bool getR1(void);
    bool getTriangle(void);
    bool getCircle(void);
    bool getCross(void);
    bool getSquare(void);
	
	byte * getBuffer(void);
	void setBuffer(byte *buffer);
	void clearBuffer(void);
	void decodeBuffer(void);
	void printBuffer(void);
    void readControllerVibrate(void);
	void readController(void);
	void setModeDigitalLock(void);
    void setModeAnalogLock(void);
    void vibrateEnable(void);
    void vibrateDisable(void);
	
    byte getRightJoyX(void);
    byte getRightJoyY(void);
    byte getLeftJoyX(void);
    byte getLeftJoyY(void);
    
  private:
    void initializeButtons(void);
    void readWrite(byte *command);
    void enterConfig(void);
    void exitConfig(void);
	void queryModelMode(void);

    byte *buffer;
    int SS;			// Slave Select Pin

    static const int PS2_BUFFER_SIZE = 9;
    static const int PS2_DELAY = 6;

    // Command constants
    const byte READ[PS2_BUFFER_SIZE] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const byte READ_VIBRATE[PS2_BUFFER_SIZE] = {0x01, 0x42, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00};
    const byte ENTER_CONFIG[PS2_BUFFER_SIZE] = {0x01, 0x43, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
    const byte EXIT_CONFIG[PS2_BUFFER_SIZE]  = {0x01, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const byte SET_MODE_DIGITAL_LOCK[PS2_BUFFER_SIZE]  = {0x01, 0x44, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00};
    const byte SET_MODE_ANALOG_LOCK[PS2_BUFFER_SIZE] = {0x01, 0x44, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00};
    const byte QUERY_MODEL_MODE[PS2_BUFFER_SIZE] = {0x01, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const byte VIBRATE_ENABLE[PS2_BUFFER_SIZE] = {0x01, 0x4D, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF};
    const byte VIBRATE_DISABLE[PS2_BUFFER_SIZE] = {0x01, 0x4D, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    // These are read from the 4th byte of controller data
    const byte SELECT_MASK = 0x01;
    const byte LEFT_JOY_BUTTON_MASK = 0x02;
    const byte RIGHT_JOY_BUTTON_MASK = 0x04;
    const byte START_MASK = 0x08;
    const byte DPAD_UP_MASK = 0x10;
    const byte DPAD_RIGHT_MASK = 0x20;
    const byte DPAD_DOWN_MASK = 0x40;
    const byte DPAD_LEFT_MASK = 0x80;

    // These are read from the 5th byte of controller data
    const byte L2_MASK = 0x01;
    const byte R2_MASK = 0x02;
    const byte L1_MASK = 0x04;
    const byte R1_MASK = 0x08;
    const byte TRIANGLE_MASK = 0x10;
    const byte CIRCLE_MASK = 0x20;
    const byte CROSS_MASK = 0x40;
    const byte SQUARE_MASK = 0x80;

    // These are read from bytes 6 -> 9 respectivly
    byte RIGHT_JOY_X;
    byte RIGHT_JOY_Y;
    byte LEFT_JOY_X;
    byte LEFT_JOY_Y;

    bool SELECT;
    bool LEFT_JOY_BUTTON;
    bool RIGHT_JOY_BUTTON;
    bool START;
    bool DPAD_UP;
    bool DPAD_RIGHT;
    bool DPAD_DOWN;
    bool DPAD_LEFT;
    bool L2;
    bool R2;
    bool L1;
    bool R1;
    bool TRIANGLE;
    bool CIRCLE;
    bool CROSS;
    bool SQUARE;
};
#endif



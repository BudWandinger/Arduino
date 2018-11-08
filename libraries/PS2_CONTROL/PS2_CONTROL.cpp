#include "PS2_CONTROL.h"

PS2_CONTROL::PS2_CONTROL(void)
{
	buffer = NULL;
	SS = NULL;
	initializeButtons();
}

PS2_CONTROL::PS2_CONTROL(int SS)
{
	buffer = NULL;
	this->SS = SS;
	initializeButtons();
}

PS2_CONTROL::~PS2_CONTROL(void)
{
	delete[] buffer;
}

void PS2_CONTROL::initializeButtons(void)
{
	SELECT = false;
	LEFT_JOY_BUTTON = false;
	RIGHT_JOY_BUTTON = false;
	START = false;
	DPAD_UP = false;
	DPAD_RIGHT = false;
	DPAD_DOWN = false;
	DPAD_LEFT = false;
	L2 = false;
	R2 = false;
	L1 = false;
	R1 = false;
	TRIANGLE = false;
	CIRCLE = false;
	CROSS = false;
	SQUARE = false;
}

void PS2_CONTROL::initializeController(void)
{
	Serial.begin(9600);
	Serial.println("Initializing PS2 Controller for Analog mode...");
	
	pinMode(SS, OUTPUT);
	digitalWrite(SS, HIGH);
	delay(100);
	
	enterConfig();
	delay(100);
	setModeAnalogLock();
	delay(100);
	exitConfig();
	delay(100);
	queryModelMode();
	delay(100);
	if(*(buffer + 1) == 0x73)
		Serial.println("Initialization successful. Controller Mode = Analog");
	else if(*(buffer + 1) == 0x41)	
		Serial.println("Initialization Failed. Controller Mode = Digital");
	else if(*(buffer + 1) == 0xF3)
		Serial.println("Initialization Failed. Controller Mode = DS2 Native");
	else 
		Serial.println("Initialization Failed. Controller Mode = UNKNOWN");
	Serial.end();
	clearBuffer();
}

bool PS2_CONTROL::getSelect(void){return SELECT;}
bool PS2_CONTROL::getLeftJoyButton(void){return LEFT_JOY_BUTTON;}
bool PS2_CONTROL::getRightJoyButton(void){return RIGHT_JOY_BUTTON;}
bool PS2_CONTROL::getStart(void){return START;}
bool PS2_CONTROL::getDpadUp(void){return DPAD_UP;}
bool PS2_CONTROL::getDpadRight(void){return DPAD_RIGHT;}
bool PS2_CONTROL::getDpadDown(void){return DPAD_DOWN;}
bool PS2_CONTROL::getDpadLeft(void){return DPAD_LEFT;}
bool PS2_CONTROL::getL2(void){return L2;}
bool PS2_CONTROL::getR2(void){return R2;}
bool PS2_CONTROL::getL1(void){return L1;}
bool PS2_CONTROL::getR1(void){return R1;}
bool PS2_CONTROL::getTriangle(void){return TRIANGLE;}
bool PS2_CONTROL::getCircle(void){return CIRCLE;}
bool PS2_CONTROL::getCross(void){return CROSS;}
bool PS2_CONTROL::getSquare(void){return SQUARE;}

byte PS2_CONTROL::getRightJoyX(void){return RIGHT_JOY_X;}
byte PS2_CONTROL::getRightJoyY(void){return RIGHT_JOY_Y;}
byte PS2_CONTROL::getLeftJoyX(void){return LEFT_JOY_X;}
byte PS2_CONTROL::getLeftJoyY(void){return LEFT_JOY_Y;}

byte * PS2_CONTROL::getBuffer(void)
{
	return buffer;
}

void PS2_CONTROL::setBuffer(byte *buffer)
{
	if(buffer != NULL)
		delete [] buffer;
	this->buffer = new byte[PS2_BUFFER_SIZE];
	for(int i = 0; i < PS2_BUFFER_SIZE; i++){
		*(this->buffer + i) = *(buffer + i);
	}
}
void PS2_CONTROL::clearBuffer(void)
{
	if(buffer != NULL){
		delete[] buffer;
		buffer = NULL;
	}
}
	
void PS2_CONTROL::readWrite(byte *command)
{
  if(buffer != NULL)
	  delete[] buffer;
  buffer = new byte[PS2_BUFFER_SIZE];
  SPI.beginTransaction(SPISettings(57600, LSBFIRST, SPI_MODE2));
  digitalWrite(SS, LOW);
  for (int i = 0; i < PS2_BUFFER_SIZE; i++) {
    *(buffer + i) = SPI.transfer(*(command + i));
    delayMicroseconds(PS2_DELAY);
  }
  digitalWrite(SS, HIGH);
  SPI.endTransaction();
}

void PS2_CONTROL::readController(void)
{
  readWrite(READ);
  decodeBuffer();
}
void PS2_CONTROL::readControllerVibrate(void)
{
  readWrite(READ_VIBRATE);
  decodeBuffer();
}

void PS2_CONTROL::decodeBuffer(void)
{	
	SELECT = not(*(buffer + 3) & SELECT_MASK);
	LEFT_JOY_BUTTON = not(*(buffer + 3) & LEFT_JOY_BUTTON_MASK);
	RIGHT_JOY_BUTTON = not(*(buffer + 3) & RIGHT_JOY_BUTTON_MASK);
	START = not(*(buffer + 3) & START_MASK);
	DPAD_UP = not(*(buffer + 3) & DPAD_UP_MASK);
	DPAD_RIGHT = not(*(buffer + 3) & DPAD_RIGHT_MASK);
	DPAD_DOWN = not(*(buffer + 3) & DPAD_DOWN_MASK);
	DPAD_LEFT = not(*(buffer + 3) & DPAD_LEFT_MASK);
	
	L2 = not(*(buffer + 4) & L2_MASK);
	R2 = not(*(buffer+ 4) & R2_MASK);
	L1 = not(*(buffer + 4) & L1_MASK);
	R1 = not(*(buffer + 4) & R1_MASK);
	TRIANGLE = not(*(buffer + 4) & TRIANGLE_MASK);
	CIRCLE = not(*(buffer + 4) & CIRCLE_MASK);
	CROSS = not(*(buffer + 4) & CROSS_MASK);
	SQUARE = not(*(buffer + 4) & SQUARE_MASK);
	
	RIGHT_JOY_X = *(buffer + 5);
	RIGHT_JOY_Y = *(buffer + 6);
	LEFT_JOY_X = *(buffer + 7);
	LEFT_JOY_Y = *(buffer + 8);
	
	if(RIGHT_JOY_Y > 127)
		RIGHT_JOY_Y = (128 - (RIGHT_JOY_Y - 127));
	else if(RIGHT_JOY_Y < 127)
		RIGHT_JOY_Y = 128 + (127 - RIGHT_JOY_Y);
	
	if(LEFT_JOY_Y > 127)
		LEFT_JOY_Y = 128 - (LEFT_JOY_Y - 127);
	else if(LEFT_JOY_Y < 127)
		LEFT_JOY_Y = 128 + (127 - LEFT_JOY_Y);
	
}

void PS2_CONTROL::enterConfig(void)
{
  readWrite(ENTER_CONFIG);
}

void PS2_CONTROL::exitConfig(void)
{
  readWrite(EXIT_CONFIG);
}

void PS2_CONTROL::setModeDigitalLock(void)
{
  readWrite(SET_MODE_DIGITAL_LOCK);
}

void PS2_CONTROL::setModeAnalogLock(void)
{
  readWrite(SET_MODE_ANALOG_LOCK);
}

void PS2_CONTROL::queryModelMode(void)
{
  readWrite(QUERY_MODEL_MODE);
}

void PS2_CONTROL::vibrateEnable(void)
{
  readWrite(VIBRATE_ENABLE);
}

void PS2_CONTROL::vibrateDisable(void)
{
  readWrite(VIBRATE_DISABLE);
}

void PS2_CONTROL::printBuffer(void)
{
  Serial.begin(9600);
  Serial.print("Buffer: ");
  for (int i = 0; i < PS2_BUFFER_SIZE; i++) {
    Serial.print(+ * (buffer + i));
    Serial.print(" ");
  }
  Serial.println();
  Serial.end();
}

#include "Nextion.h"
#include "DHT.h";

// define the GPIO pins used for specified hardware
#define TempHumSensorGPIO 2
#define FanGPIO 3
#define HeatGPIO 4
#define VoltSensorGPIO A0

// the page ID for the GUI
#define PAGE_ID 0

// the measured temperature text window on the GUI
#define MEAS_TEMP_TXT_ID 5
#define MEAS_TEMP_TXT_NAME "t0"

// the measured temperature value window on the GUI
#define MEAS_TEMP_VAL_ID 6
#define MEAS_TEMP_VAL_NAME "n0"

// the selected temperature text window on the GUI
#define SEL_TEMP_TXT_ID 2
#define SEL_TEMP_TXT_NAME "t1"

// the selected temperature value window on the GUI
#define SEL_TEMP_VAL_ID 7
#define SEL_TEMP_VAL_NAME "n1"

// the increase temperature button on the GUI
#define INC_TEMP_BTN_ID 3
#define INC_TEMP_BTN_NAME "b0"

// the decrease temperature button on the GUI
#define DEC_TEMP_BTN_ID 4
#define DEC_TEMP_BTN_NAME "b1"

// the measured humidity text window on the GUI
#define MEAS_HUM_TXT_ID 8
#define MEAS_HUM_TXT_NAME "t2"

// the measured humidity value window on the GUI
#define MEAS_HUM_VAL_ID 10
#define MEAS_HUM_VAL_NAME "n2"

// the selected humidity text window on the GUI
#define SEL_HUM_TXT_ID 9
#define SEL_HUM_TXT_NAME "t3"

// the selected humidity value window on the GUI
#define SEL_HUM_VAL_ID 11
#define SEL_HUM_VAL_NAME "n3"

// the increase humidity button on the GUI
#define INC_HUM_BTN_ID 12
#define INC_HUM_BTN_NAME "b2"

// the decrease humidity button on the GUI
#define DEC_HUM_BTN_ID 13
#define DEC_HUM_BTN_NAME "b3"

// the measured voltage text window on the GUI 
#define MEAS_VOLT_TXT_ID 14
#define MEAS_VOLT_TXT_NAME "t4"

// the measured voltage value window on the GUI
#define MEAS_VOLT_VAL_ID 15
#define MEAS_VOLT_VAL_NAME "n4"

// declaration of the callback functions for the GUI buttons
void incTempBtnPopCallback(void *ptr);
void decTempBtnPopCallback(void *ptr);
void incHumBtnPopCallback(void *ptr);
void decHumBtnPopCallback(void *ptr);

// declaration of voltage measureing function
float measureVoltage(void);

// the number window objectes for the GUI
NexNumber selTempNum = NexNumber(PAGE_ID, SEL_TEMP_VAL_ID, SEL_TEMP_VAL_NAME);
NexNumber measTempNum = NexNumber(PAGE_ID, MEAS_TEMP_VAL_ID, MEAS_TEMP_VAL_NAME);
NexNumber selHumNum = NexNumber(PAGE_ID, SEL_HUM_VAL_ID, SEL_HUM_VAL_NAME);
NexNumber measHumNum = NexNumber(PAGE_ID, MEAS_HUM_VAL_ID, MEAS_HUM_VAL_NAME);
NexNumber measVoltNum = NexNumber(PAGE_ID, MEAS_VOLT_VAL_ID, MEAS_VOLT_VAL_NAME);

// the button objects for the GUI
NexButton incTempBtn = NexButton(PAGE_ID, INC_TEMP_BTN_ID, INC_TEMP_BTN_NAME);
NexButton decTempBtn = NexButton(PAGE_ID, DEC_TEMP_BTN_ID, DEC_TEMP_BTN_NAME);
NexButton incHumBtn = NexButton(PAGE_ID, INC_HUM_BTN_ID, INC_HUM_BTN_NAME);
NexButton decHumBtn = NexButton(PAGE_ID, DEC_HUM_BTN_ID, DEC_HUM_BTN_NAME);

// the temperature/humidity sensor object
DHT tempHumSensor(TempHumSensorGPIO, DHT22);

// the selected and measured temperature and humidity and voltage values
float selTemp = 20.0;
float measTemp = NULL;
float selHum = 60.0;
float measHum = NULL;
float measVolt = NULL;

// the listening list for button pressed from the GUI
NexTouch *nex_listen_list[] = 
{
  &incTempBtn,
  &decTempBtn,
  &incHumBtn,
  &decHumBtn,
  NULL
};

// the implementation for the increase temperature button callback
void incTempBtnPopCallback(void *ptr)
{
  //dbSerialPrintln("incTempButtonPopCallback");
  selTemp += 1.0f;
  selTempNum.setValue(selTemp);
}

// the implementation for the decrease temperature button callback
void decTempBtnPopCallback(void *ptr)
{
  //dbSerialPrintln("decTempButtonPopCallback");
  selTemp -= 1.0f;
  selTempNum.setValue(selTemp);
}

// the implementation for the increase humidity button callback
void incHumBtnPopCallback(void *ptr)
{
  selHum += 1.0f;
  selHumNum.setValue(selHum);
}

// the implementation for the decrease humidity button callback
void decHumBtnPopCallback(void *ptr)
{
  selHum -= 1.0f;
  selHumNum.setValue(selHum);
}

// implementation of the measure voltage function
float measureVoltage(void)
{
  // take average of 20 measurements to get an acurate measurement 
  float voltageSum = 0;
  for(int i = 0; i < 20; i++)
  {
    // convert analog read with 1024-bit resolution to actual voltage
    float rawInput = analogRead(VoltSensorGPIO);
    float percent = rawInput/1023;
    float voltage = percent * 5.0f;
    voltageSum += voltage;
  }
  float averageVoltage = voltageSum/20;

  return averageVoltage;
}

// the setup function for arduino, only done once on power cycle
void setup(void) 
{
  // define GPIO pin modes
  pinMode(VoltSensorGPIO, INPUT);
  pinMode(FanGPIO, OUTPUT);
  pinMode(HeatGPIO, OUTPUT);

  // initialize the GUI
  nexInit(); 

  // initialize the selected numbers for the GUI to the default values
  selTempNum.setValue(selTemp);
  selHumNum.setValue(selHum);
  
  // attach the increase and decrease temperature and humidity 
  // callback functions to the listeners
  incTempBtn.attachPop(incTempBtnPopCallback);
  decTempBtn.attachPop(decTempBtnPopCallback);
  incHumBtn.attachPop(incHumBtnPopCallback);
  decHumBtn.attachPop(decHumBtnPopCallback);
  
  //dbSerialPrintln("setup done");
}

void loop() 
{
  // check listener list for GUI button presses
  nexLoop(nex_listen_list);
  
  // measure the temperature and update the numbers on the GUI
  uint32_t newMeasTemp = uint32_t(tempHumSensor.readTemperature());
  if(newMeasTemp != measTemp)
  {
    measTemp = newMeasTemp;
    measTempNum.setValue(measTemp);
  }

  // measure the humidity and update the numbers on the GUI
  uint32_t newMeasHum = uint32_t(tempHumSensor.readHumidity());
  if(newMeasHum != measHum)
  {
    measHum = newMeasHum;
    measHumNum.setValue(measHum);
  }
  
  // measure the voltage and update number on the GUI
  uint32_t newMeasVolt = uint32_t(measureVoltage());
  if(newMeasVolt != measVolt)
  {
    measVolt = newMeasVolt;
    measVoltNum.setValue(measVolt);
  }

  // manage fan and heater hardware
  if(measTemp > selTemp)
  {
    digitalWrite(FanGPIO, HIGH);
    digitalWrite(HeatGPIO, HIGH);
  }
  else if(measHum > selHum)
  {
    digitalWrite(FanGPIO, HIGH);
    digitalWrite(HeatGPIO, LOW);
  }
  else
  {
    digitalWrite(FanGPIO, LOW);
    digitalWrite(HeatGPIO, LOW);
  }
}


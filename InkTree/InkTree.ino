#include <MIDIUSB.h>

/* CONFIGURATION */
//#define SERIAL_MONITOR
#define STATE_MACHINE_DEBUG
//#define MIDI_MESSAGE_DEBUG
//#define TIMER_DEBUG
//#define RANGE_SENSOR_DEBUG
//#define MOTION_SENSOR_DEBUG

/* CONSTANTS */
#define RANGE_SENSOR_PIN              (uint8_t)A0
#define MOTION_SENSOR_PIN             (uint8_t)2
#define RANGE_ARRAY_SIZE              (uint8_t)10
#define RANGE_UPDATE_INTERVAL         (uint8_t)50
#define RANGE_SENSOR_TRIGGER_ON       (uint16_t)300
#define RANGE_SENSOR_TRIGGER_OFF      (uint16_t)200
#define SENSOR_TRIGGER_TIMER          (uint32_t)120000  /* 2 minutes */
#define MINIMUM_MIDI_INTERVAL         (uint8_t)100
#define MAXIMUM_MIDI_INTERVAL         (uint32_t)6000000   /* 10 minutes */
#if defined(SERIAL_MONITOR) || defined(STATE_MACHINE_DEBUG) || defined(MIDI_MESSAGE_DEBUG) || defined(TIMER_DEBUG)
#define SERIAL_BAUD                   (uint16_t)115200
#endif

/* TYPEDEFS */
typedef enum {
  OFF,
  GROW_TREE,
  FLASH_TREE,
  SOLID_TREE
} stateType;

/* VARIABLES */
static unsigned long timeOfLastMidiMsg;
static unsigned long timeOfLastSensorTrigger;
static unsigned long timeOfLastRangeUpdate;
static bool rangeSensorTriggered;
static stateType state = OFF;
static uint16_t range;
static uint16_t rangeArray[RANGE_ARRAY_SIZE];
static uint8_t rangeArrayIndex;

/* PRIVATE FUNCTION PROTOTYPES */
static bool motionSensorTrigger(void);
static bool rangeSensorTrigger(void);
static uint32_t calcDeltaTime(uint32_t timeA_, uint32_t timeB_);
static void midiCommandStateChange(byte state_);
static void initializeRangeArray(void);
static void updateRangeArray(void);
static void updateRange(void);

/* MAIN INIT */
void setup()
{
  #if defined(SERIAL_MONITOR) || defined(STATE_MACHINE_DEBUG) || defined(MIDI_MESSAGE_DEBUG) || defined(TIMER_DEBUG)
    /* Initilaize the serial monitor port. */
    Serial.begin(SERIAL_BAUD);
  #endif

  #if defined(SERIAL_MONITOR)
    /* Report start of initialization to serial port. */
    Serial.println("Initializing range sensor...");
  #endif

  /* Initialize the motion and range sensor pins to inputs. */
  pinMode(RANGE_SENSOR_PIN, INPUT);
  pinMode(MOTION_SENSOR_PIN, INPUT);

  /* Initialize static variables. */
  state = OFF;
  rangeSensorTriggered = false;
  timeOfLastSensorTrigger = millis();

  /* Initialize range array and range variables. */
  range = 0;
  initializeRangeArray();
  updateRange();
  
  /* Initialize light controller with state OFF. */
  midiCommandStateChange((byte)OFF);

  #if defined SERIAL_MONITOR
    /* Report initialization complete to user. */
    Serial.println("\tCOMPLETE");
  #endif
}

/* MAIN LOOP */
void loop()
{
  /* Update the range array at its required interval. */
  if(calcDeltaTime(timeOfLastRangeUpdate, millis() >= RANGE_UPDATE_INTERVAL))
  {
    updateRangeArray();
    updateRange();
  }
  
  /* Move through the state machine at the minimum Midi message interval. */
  if (calcDeltaTime(timeOfLastMidiMsg, millis()) >= MINIMUM_MIDI_INTERVAL)
  {
    /* Move through another iteration of the state machine. */
    switch (state)
    {
      case OFF:
        #if defined(STATE_MACHINE_DEBUG)
          Serial.println("STATE: OFF");
        #endif
        if (motionSensorTrigger())
        {
          /* Send the midi msg associated with this state change. */
          midiCommandStateChange((byte)GROW_TREE);

          /* Change the state of the state machine */
          state = GROW_TREE;

          #if defined(STATE_MACHINE_DEBUG)
            /* Report state change over serial console. */
            Serial.println("***** STATE CHANGE: OFF -> GROW_TREE *****");
          #endif
        }
        else if (calcDeltaTime(timeOfLastMidiMsg, millis()) >= MAXIMUM_MIDI_INTERVAL)
        {
          /* Send a repeat midi message for this state to keep the connection to control software alive. */
          midiCommandStateChange((byte)OFF);
        }
        else
        {
          /* Stay in the OFF state until the motion sensor gets triggered. */
        }
        break;

      case GROW_TREE:
        #if defined(STATE_MACHINE_DEBUG)
          Serial.println("STATE: GROW_TREE");
        #endif
        if (rangeSensorTrigger())
        {
          /* Send the midi msg associated with this state change. */
          midiCommandStateChange((byte)FLASH_TREE);

          /* Change the state of the state machine. */
          state = FLASH_TREE;

          #if defined(STATE_MACHINE_DEBUG)
            /* Report state change over serial console. */
            Serial.println("***** STATE CHANGE: GROW_TREE -> FLASH_TREE *****");
          #endif
        }
        else if (calcDeltaTime(timeOfLastSensorTrigger, millis()) >= SENSOR_TRIGGER_TIMER)
        {
          /* Send the midi msg associated with this state change. */
          midiCommandStateChange((byte)OFF);

          /* Change the state of the state machine. */
          state = OFF;

          #if defined(STATE_MACHINE_DEBUG)
            /* Report state change over serial console. */
            Serial.println("***** STATE CHANGE: GROW_TREE -> OFF *****");
          #endif
        }
        else
        {
          /* Stay in the GROW_TREE state until either the range sensor is triggered or the timer expires. */
        }
        break;

      case FLASH_TREE:
        #if defined(STATE_MACHINE_DEBUG)
          Serial.println("STATE: FLASH_TREE");
        #endif
        if (!rangeSensorTrigger())
        {
          /* Send the midi msg associated with this state change. */
          midiCommandStateChange((byte)SOLID_TREE);

          /* Change the state of the state machine */
          state = SOLID_TREE;

          #if defined(STATE_MACHINE_DEBUG)
            /* Report state change over serial console. */
            Serial.println("***** STATE CHANGE: FLASH_TREE -> SOLID_TREE *****");
          #endif
        }
        else
        {
          /* Stay in this state until the range sensor stops being triggered. */
        }
        break;

      case SOLID_TREE:
        #if defined(STATE_MACHINE_DEBUG)
          Serial.println("STATE: SOLID_TREE");
        #endif
        if (rangeSensorTrigger())
        {
          /* Send the midi msg associated with this state change. */
          midiCommandStateChange((byte)FLASH_TREE);

          /* Change the state of the state machine. */
          state = FLASH_TREE;

          #if defined(STATE_MACHINE_DEBUG)
            /* Report state change over serial console. */
            Serial.println("***** STATE CHANGE: SOLID_TREE -> FLASH_TREE *****");
          #endif
        }
        else if (motionSensorTrigger())
        {
          /* Stay in the SOLID_TREE state until either the range sensor is triggered or the timer expires. */
        }
        else if (calcDeltaTime(timeOfLastSensorTrigger, millis()) >= SENSOR_TRIGGER_TIMER)
        {
          /* Send the midi msg associated with this state change. */
          midiCommandStateChange((byte)OFF);

          /* Change the state of the state machine. */
          state = OFF;

          #if defined(STATE_MACHINE_DEBUG)
            /* Report state change over serial console. */
            Serial.println("***** STATE CHANGE: SOLID_TREE -> OFF_TREE *****");
          #endif
        }
        break;
    }
  }
}

/*
   Reads from the motion sensor and returns true if the motion sensor value is high, otherwise false.
*/
static bool motionSensorTrigger(void)
{
  if (digitalRead(MOTION_SENSOR_PIN))
  {
    #if defined(MOTION_SENSOR_DEBUG)
      Serial.println("*** MOTION SENSOR TRIGGER ***");
    #endif

    timeOfLastSensorTrigger = millis();

    return true;
  }
  else
  {
    return false;
  }
}

/*
 * Compares the range value read from the sensor with the RANGE_SENSOR_TRIGER_VALUE to see if it has
 * been triggered or not.
 */
static bool rangeSensorTrigger(void)
{
#if defined(RANGE_SENSOR_DEBUG)
  Serial.print("*** RANGE: ");
  Serial.print(analogRead(RANGE_SENSOR_PIN));
  Serial.println(" ***");
#endif

  if (rangeSensorTriggered == false)
  {
    if (range >= RANGE_SENSOR_TRIGGER_ON)
    {
      #if defined(RANGE_SENSOR_DEBUG)
        Serial.println("*** RANGE SENSOR TRIGGER: ON ***");
      #endif

      rangeSensorTriggered = true;
      timeOfLastSensorTrigger = millis();
      return true;
    }
    else
    {
      return false; 
    }
  }
  else if (rangeSensorTriggered == true)
  {
    if (range >= RANGE_SENSOR_TRIGGER_OFF)
    {
      #if defined(RANGE_SENSOR_DEBUG)
        Serial.println("*** RANGE SENSOR TRIGGER ***");
      #endif

      timeOfLastSensorTrigger = millis();
      return true;
    }
    else
    {
      #if defined(RANGE_SENSOR_DEBUG)
        Serial.println("*** RANGE SENSOR TRIGGER: OFF ***");
      #endif

      rangeSensorTriggered = false;
      return false;
    }
  }
}

/*
 * Initializes the rangeArray by reading values from the range sensor and adding them to the array.
 */
void initializeRangeArray(void)
{
  #if defined(SERIAL_MONITOR)
    Serial.println("Initializing range array...");
  #endif

  rangeArrayIndex = 0;
  
  while (rangeArrayIndex < RANGE_ARRAY_SIZE)
  {
    /* delay for 50ms for the max frequency of the range sensor */
    delay(50);

    rangeArray[rangeArrayIndex] = (uint16_t)analogRead(RANGE_SENSOR_PIN);
    rangeArrayIndex++;
  }

  #if defined(SERIAL_MONITOR)
    Serial.println("Complete.");
  #endif
}

/*
 * Updates the rangeArray by taking a single range measurment from the range sensor and then adding it to
 * the rangeArray by writing over th oldest measurement in the rangeArray.
 */
void updateRangeArray(void)
{
  /* progress rangeArrayIndex or reset if index is at its max */
  if (rangeArrayIndex >= RANGE_ARRAY_SIZE)
  {
    rangeArrayIndex = 0;
  }
  else
  {
    rangeArrayIndex++;
  }

  /* read data from range sensor, scale it, and add it to the rangeArray */
  rangeArray[rangeArrayIndex] = (uint16_t)analogRead(RANGE_SENSOR_PIN);
}

/*
 * Calculates the average of the rangeArray which produces a filtered range value, and then update the
 * range variable with this filtered value.
 */
void updateRange(void)
{
  uint16_t rangeArraySum = 0;

  for (uint8_t i = 0; i < RANGE_ARRAY_SIZE; i++)
  {
    rangeArraySum += rangeArray[i];
  }

  range = rangeArraySum / RANGE_ARRAY_SIZE;
}

/*
   Calculates and returns the diffence in milliseconds between timeA_ and timeB_: timeB_ - timeA_.
*/
static uint32_t calcDeltaTime(uint32_t timeA_, uint32_t timeB_)
{
  uint32_t deltaTime;

  if (timeB_ >= timeA_)
  {
    deltaTime = timeB_ - timeA_;
  }
  else
  {
    deltaTime = timeB_ + (0xFFFFFFFFFFFFFFFF - timeB_);
  }

#if defined(TIMER_DEBUG)
  Serial.print("TIMER: timeB_: ");
  Serial.print(timeB_);
  Serial.print("  -  timeA_: ");
  Serial.print(timeA_);
  Serial.print("  =  delta time: ");
  Serial.println(deltaTime);
#endif

  return deltaTime;
}

/*
   Sends midi commands and updates the midi message time tracking variable to track at what time the
   last Midi message was sent.
*/
static void midiCommandStateChange(byte state_)
{
  /* Send the new MIDI message */
  midiEventPacket_t event = {0x0B, 0xB0 | 0, state_, 127};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();

  /* Update the midi time tracking variable. */
  timeOfLastMidiMsg = millis();

#if defined(MIDI_MESSAGE_DEBUG)
  Serial.print("**** MIDI MESSAGE SENT: ");
  Serial.print(state_);
  Serial.println(" *****");
#endif
}

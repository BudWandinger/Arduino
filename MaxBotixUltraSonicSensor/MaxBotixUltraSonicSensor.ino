
#include <MIDIUSB.h>

/* configurations */
#define SERIAL_MONITOR  (bool)true

/* constants */
#define RANGE_SENSOR_PIN            (uint8_t)A0       /* the physical pin connected to the range sensor */
#define MOTION_SENSOR_PIN           (uint8_t)2        /* the physical pin connected to thje range sensor */
#define RANGE_ARRAY_SIZE            (uint8_t)20       /* the size of the filtering array for the renge sensor */
#define NUM_REGULAR_EVENTS          (uint8_t)7        /* the number of unique regular events that can be trigered by the motion sensor */
#define NUM_SPECIAL_EVENTS          (uint8_t)7        /* the number of unique special events that can be trigered by the range sensor */
#define RANGE_SENSOR_TRIGER_VALUE   (uint8_t)95       /* the range at which a special event will be triggered */
#define MOTION_TRIGER_TIMER_MAX     (uint16_t)120000   /* the motion trigger max timer in ms */

#ifdef SERIAL_MONITOR
#define SERIAL_BAUD   (uint16_t)115200
#endif /* ifdef SERIAL_MONITOR */

/* global variable */
uint8_t G_rangeArray[RANGE_ARRAY_SIZE];
uint8_t G_rangeArrayIndex;
uint8_t G_filteredRange;

/* function definitions */
void initializeRangeArray(void);
void updateRangeArray(void);
void updateRange(void);
bool rangeSensorTriger(void);
bool motionSensorTriger(void);
void controlChange(byte channel, byte control, byte value);

void setup() {

  /* initialize the global variables */
  G_rangeArrayIndex = 0;
  G_filteredRange = 0;

#ifdef SERIAL_MONITOR
  /* initilaize the serial monitor port and report to user */
  Serial.begin(115200);
  Serial.print("Initializing Range Sensor...");
#endif /* ifdef SERIAL_MONITOR */

  /* initialize the motion and range sensor pins to inputs */
  pinMode(RANGE_SENSOR_PIN, INPUT);
  pinMode(MOTION_SENSOR_PIN, INPUT);

  /* intialize the G_rangeArray with measured range values  and then update G_range variable*/
  initializeRangeArray();
  updateRange();

#ifdef SERIAL_MONITOR
  /* report initialization complete to user */
  Serial.println("\tCOMPLETE");
#endif /* ifdef SERIAL_MONITOR */

}

void loop() {

  /* initialize both sensor triggers to false */
  static bool motionTrigerFlag = false;
  static bool rangeTrigerFlag = false;

  /* initialize the event tracking variables */
  static uint8_t  currentRegularEvent = 0;
  static uint8_t  currentSpecialEvent = 0;
  static uint16_t motionTrigerTimer = 0;
  /* event 127 is invalid, so no matter what the first event is, it will be different than this initialized value */
  static uint8_t  lastSentEvent = 127;

  /* delay required for max frequency of range sensor = 50ms */
  delay(50);

  /* update the rangeArray with a new value and then update the global G_range variable with a filtered value */
  updateRangeArray();
  updateRange();

#ifdef SERIAL_MONITOR
  /* print the filtered range to the serial monitor for debugging */
  Serial.print("Filtered Range: " );
  Serial.println(G_filteredRange);
  Serial.print("Range Triger: ");
  if (rangeSensorTriger())
  {
    Serial.println("TRUE");
  }
  else
  {
    Serial.println("FALSE");
  }
  Serial.print("Motion Triger: ");
  if (motionSensorTriger())
  {
    Serial.println("TRUE");
  }
  else
  {
    Serial.println("FALSE");
  }
#endif /* ifdef SERIAL_MONITOR */

  /* logic for setting the motionSensorFlag   */
  if (motionSensorTriger())
  {
    /* if the motionTriggerFlag was not previously set */
    if (motionTrigerFlag == false)
    {
      /* set the motionTrigerFlag */
      motionTrigerFlag = true;
    }
    else
    {
      /* keep resetting the motionTriggerTimer while the motionSensorTrigger is high */
      motionTrigerTimer = 0;
    }
  }
  else
  {
    /*  if the motionSensorTriger is no longer high but the motionTrigerFlag is still set, keep the
        motionTrigerFlag set until the motionSensorTimer expires */
    if (motionTrigerFlag == true)
    {
      if (motionTrigerTimer >= MOTION_TRIGER_TIMER_MAX)
      {
        motionTrigerTimer = 0;
        motionTrigerFlag = false;

        /*  after releasing the motionTrigerFlag, progress the currentRegularEvent so that a new regular
            event is trigered next time the motionTrigerFlag is set */
        if (currentRegularEvent == NUM_REGULAR_EVENTS - 1)
        {
          currentRegularEvent = 0;
        }
        else
        {
          currentRegularEvent++;
        }
      }
      else
      {
        motionTrigerTimer += 50; /* the loop section of this sketch will run approx. every 50ms */
      }
    }
  }


  /* logic for setting the rangeSensorTriger */
  if (rangeSensorTriger())
  {
    if (rangeTrigerFlag == false)
    {
      rangeTrigerFlag = true;
    }
  }
  else
  {
    /* if the rangeSensorTriger is no longer high and the rangeTrigerFlag was previously set,
        progress the currentSpecialEvent variable so that a new special event is trigered next
        time the rangeTrigerFlag is set */
    if (rangeTrigerFlag == true)
    {
      rangeTrigerFlag = false;

      if (currentSpecialEvent == NUM_SPECIAL_EVENTS - 1)
      {
        currentSpecialEvent = 0;
      }
      else
      {
        currentSpecialEvent++;
      }
    }
  }


  /* handle motionTrigerFlag */
  if (motionTrigerFlag == true)
  {
    /* handle rangeTrigerFlag: rangeTrigerFlag supersedes motionTrigerFlag */
    if (rangeTrigerFlag == true)
    {
      /* only send the event over MIDI if it is a new event, this reduces the MIDI bandwidth used */
      if (currentSpecialEvent + NUM_REGULAR_EVENTS + 1!= lastSentEvent)
      {
        /* send the event number on chanel 1 (0) with maximum velocity (127) */
        controlChange(0, currentSpecialEvent + NUM_REGULAR_EVENTS + 1, 127);
        MidiUSB.flush();

        /* update the lastSentEvent variable with the event that was just sent */
        lastSentEvent = currentSpecialEvent + NUM_REGULAR_EVENTS + 1;
      }
    }
    else
    {
      /* if the rangeTrigerFlag is not set but the motionTrigerFlag is, send the regular event */
      if (currentRegularEvent + 1 != lastSentEvent)
      {
        /* send the event number on chanel 1 (0) with maximum velocity (127) */
        controlChange(0, currentRegularEvent + 1, 127);
        MidiUSB.flush();

        /* update the lastSentEvent variable with the event that was just sent */
        lastSentEvent = currentRegularEvent + 1;
      }
    }
  }
  /* if no the motionTrigerFlag is not set, send the default state over MIDI */
  else
  {
    /* send the default event number (0) on chanel 1 (0) with maximum velocity (127) */
    controlChange(0, 0, 127);
    MidiUSB.flush();

    /* update the lastSentEvent variable with the event that was just sent */
    lastSentEvent = 0;
  }

#ifdef SERIAL_MONITOR
  Serial.print("Last Sent Event: ");
  Serial.println(lastSentEvent);
  Serial.println();
#endif /*ifdef SERIAL_MONITOR */

}

/*
   Initializes the rangeArray by reading values from the range sensor and adding them to the array.
*/
void initializeRangeArray(void)
{
  while (G_rangeArrayIndex < RANGE_ARRAY_SIZE)
  {
    /* delay for 50ms for the max frequency of the range sensor */
    delay(50);

    G_rangeArray[G_rangeArrayIndex] = (uint16_t)analogRead(RANGE_SENSOR_PIN);
    G_rangeArrayIndex++;
  }
}

/*
   Updates the rangeArray by taking a single range measurment from the range sensor and then adding it to
   the rangeArray by writing over th oldest measurement in the rangeArray.
*/
void updateRangeArray(void)
{
  /* progress rangeArrayIndex or reset if index is at its max */
  if (G_rangeArrayIndex >= RANGE_ARRAY_SIZE)
  {
    G_rangeArrayIndex = 0;
  }
  else
  {
    G_rangeArrayIndex++;
  }

  /* read data from range sensor, scale it, and add it to the rangeArray */
  G_rangeArray[G_rangeArrayIndex] = (uint8_t)analogRead(RANGE_SENSOR_PIN);
}

/*
   Calculates the average of the rangeArray which produces a filtered range value, and then update the
   global G_range vfariable with this filtered value.
*/
void updateRange(void)
{
  uint16_t rangeArraySum = 0;

  for (uint8_t i = 0; i < RANGE_ARRAY_SIZE; i++)
  {
    rangeArraySum += G_rangeArray[i];
  }

  G_filteredRange = rangeArraySum / RANGE_ARRAY_SIZE;
}

/*
   Compares the G_range value with the RANGE_SENSOR_TRIGER_VALUE, if the G_range calue is less than or equal to
   the RANGE_SENSOR_TRIGER_VALUE the rangeSensorTriger is considered true, otherwise false.
*/
bool rangeSensorTriger(void)
{
  if (G_filteredRange <= RANGE_SENSOR_TRIGER_VALUE)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/*
   Reads from the motion sensor and returns true if the motion sensor value is high, otherwise false.
*/
bool motionSensorTriger(void)
{
  if (digitalRead(MOTION_SENSOR_PIN))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/*
   Sends midi comands.
*/
void controlChange(byte channel, byte control, byte value)
{
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

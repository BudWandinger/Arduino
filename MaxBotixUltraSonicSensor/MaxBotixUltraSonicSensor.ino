
#include <MIDIUSB.h>

#define RANGE_SENSOR_PIN  (uint8_t)A0
#define RANGE_ARRAY_SIZE  (uint8_t)100

uint8_t rangeArray[RANGE_ARRAY_SIZE];
uint8_t rangeArrayIndex = 0;

void setup() {

  Serial.begin(115200);
  Serial.print("Initializing Range Sensor...");
  
  pinMode(RANGE_SENSOR_PIN, INPUT);

  /* fill the range array */
  while(rangeArrayIndex < RANGE_ARRAY_SIZE)
  {
    delay(50);
    uint16_t unscaledRange = (uint16_t)analogRead(RANGE_SENSOR_PIN);
    rangeArray[rangeArrayIndex] = scaleRange(unscaledRange);
    rangeArrayIndex++;
  }

  Serial.println("\tCOMPLETE");
}

void loop() {

  /* used to determin if a new range must be send via MIDI */
  static uint8_t lastRange = 0;
  
  /* delay required for max frequency of range sensor = 50ms */
  delay(50);

  /* progress rangeArrayIndex or reset if index is at its max */
  if(rangeArrayIndex >= RANGE_ARRAY_SIZE)
  {
    rangeArrayIndex = 0;
  }
  else
  {
    rangeArrayIndex++;
  }

  /* read data from range sensor, scale it, and add it to the rangeArray */
  uint16_t unscaledRange = (uint16_t)analogRead(RANGE_SENSOR_PIN);
  rangeArray[rangeArrayIndex] = scaleRange(unscaledRange);

  /* calculate the average of the rangeArray */
  uint16_t rangeArraySum = 0;
  for(uint8_t i = 0; i < RANGE_ARRAY_SIZE; i++)
  {
    rangeArraySum += rangeArray[i];
  }
  uint8_t filteredRange = rangeArraySum / RANGE_ARRAY_SIZE;

  Serial.print("Filtered Range: " );
  Serial.println(filteredRange);

  /* if filteredRange is new, send the new value via MIDI and update lastRange */
  if(filteredRange != lastRange)
  {
    controlChange(0, 48, filteredRange); // chanel 1, note 48 (middle c), velocity 127
    MidiUSB.flush();

    lastRange = filteredRange;
  }

}

/* takes a range value from 0 - 1023 and scales it to a value from 0 -127 */
uint8_t scaleRange(uint16_t unscaledRange_)
{
  uint8_t scaledRange = ( (unscaledRange_ / 1023) * 127 ) ;
  return scaledRange;
}

void controlChange(byte channel, byte control, byte value) 
{
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

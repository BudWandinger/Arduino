#define RANGE_SENSOR_PIN  (uint8_t)A0
#define RANGE_ARRAY_SIZE  (uint16_t)100

int rangeArray[RANGE_ARRAY_SIZE];
int rangeArrayIndex = 0;

void setup() {

  Serial.begin(9600);
  Serial.print("Initializing Range Sensor...");
  
  pinMode(RANGE_SENSOR_PIN, INPUT);

  /* fill the range array */
  while(rangeArrayIndex < RANGE_ARRAY_SIZE)
  {
    delay(50);
    rangeArray[rangeArrayIndex] = analogRead(RANGE_SENSOR_PIN);
    rangeArrayIndex++;
  }

  Serial.println("\tCOMPLETE");
}

void loop() {
  
  delay(50);

  if(rangeArrayIndex >= RANGE_ARRAY_SIZE)
  {
    rangeArrayIndex = 0;
  }
  else
  {
    rangeArrayIndex++;
  }
  rangeArray[rangeArrayIndex] = analogRead(RANGE_SENSOR_PIN);

  int filteredRange = 0;
  for(int i = 0; i < RANGE_ARRAY_SIZE; i++)
  {
    filteredRange += rangeArray[i];
  }
  filteredRange /= RANGE_ARRAY_SIZE;

  Serial.print("Filtered Range: " );
  Serial.println(filteredRange);

}

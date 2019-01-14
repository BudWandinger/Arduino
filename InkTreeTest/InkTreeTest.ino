#define MOTION_TRIGGER_PIN  (uint8_t)2
#define RANGE_TRIGGER_PIN   (uint8_t)3

void setup()
{
  pinMode(MOTION_TRIGGER_PIN, OUTPUT);
  pinMode(RANGE_TRIGGER_PIN, OUTPUT);

  digitalWrite(MOTION_TRIGGER_PIN, LOW);
  digitalWrite(RANGE_TRIGGER_PIN, LOW);
}

void loop()
{
  // go to state 1 and back to state 0
  digitalWrite(MOTION_TRIGGER_PIN, HIGH);
  delay(1000);
  digitalWrite(MOTION_TRIGGER_PIN, LOW);
  delay(120100);

  // go to state one and then state two really fast
  digitalWrite(MOTION_TRIGGER_PIN, HIGH);
  delay(100);
  digitalWrite(RANGE_TRIGGER_PIN, HIGH);
  delay(1000);
  digitalWrite(MOTION_TRIGGER_PIN, LOW);
  digitalWrite(RANGE_TRIGGER_PIN, LOW);
  delay(120100);

  // flash the motion trigger pin really fast on and off
  for(int i = 0; i < 10000; i++)
  {
    digitalWrite(MOTION_TRIGGER_PIN, HIGH);
    delay(1);
    digitalWrite(MOTION_TRIGGER_PIN, LOW);
    delay(1);
  }
  delay(120100);

  // go to state one and flash the range sensor pin on and off really fast
  digitalWrite(MOTION_TRIGGER_PIN, HIGH);
  for(int i = 0; i < 10000; i++)
  {
    digitalWrite(RANGE_TRIGGER_PIN, HIGH);
    delay(1);
    digitalWrite(RANGE_TRIGGER_PIN, LOW);
    delay(1);
  }
  digitalWrite(MOTION_TRIGGER_PIN, LOW);
  delay(120100);

  // move between state 1 and 2 at the maximum midi interval
  digitalWrite(MOTION_TRIGGER_PIN, HIGH);
  for(int i = 0; i < 1000; i++)
  {
    delay(100);
    digitalWrite(RANGE_TRIGGER_PIN, HIGH);
    delay(100);
    digitalWrite(RANGE_TRIGGER_PIN, LOW);
  }
  digitalWrite(MOTION_TRIGGER_PIN, LOW);
  delay(120100);

  // flash the range trigger pin really fast 
  for(int i = 0; i < 10000; i++)
  {
    digitalWrite(RANGE_TRIGGER_PIN, HIGH);
    delay(1);
    digitalWrite(RANGE_TRIGGER_PIN, LOW);
    delay(1);
  }
  delay(120100);

  // flash both triggers really randomly for a while
  for(int i = 0; i < 500; i++)
  {
    digitalWrite(RANGE_TRIGGER_PIN, HIGH);
    delay(122);
    digitalWrite(MOTION_TRIGGER_PIN, HIGH);
    delay(245);
    digitalWrite(MOTION_TRIGGER_PIN, LOW);
    delay(111);
    digitalWrite(MOTION_TRIGGER_PIN, HIGH);
    delay(467);
    digitalWrite(RANGE_TRIGGER_PIN, LOW);
    delay(231);
    digitalWrite(RANGE_TRIGGER_PIN, HIGH);
    digitalWrite(MOTION_TRIGGER_PIN, LOW);
    delay(212);
    digitalWrite(RANGE_TRIGGER_PIN, LOW);
  }
  
  // do nothing for a long time
  delay(1200000);
  
}

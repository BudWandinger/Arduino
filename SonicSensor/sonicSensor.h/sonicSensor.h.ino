class SonicSensor {
  public:
    SonicSensor(long margin);
    long distance();
  private:
    int SS_trig;
    int SS_echo;
    long margin;
    long recentDistances[10];
    long *head;
    long averageDistance;
    bool withinMargin(long);
};

SonicSensor::SonicSensor(int SS_trig, int SS_echo, long margin) {
  this->SS_trig = SS_trig;
  this->SS_echo = SS_echo;
  for (int i = 0; i < 10; i++)
    recentDistances[i] = i;
  averageDistance = 0;
  head = recentDistances;
  this->margin = margin;

  pinMode(SS_trig, OUTPUT);
  pinMode(SS_echo, INPUT);
}

bool SonicSensor::withinMargin(long thisDistance)
{
  long previousAverage = averageDistance;

  if (head == &recentDistances[0])
    head = recentDistances + 9;
  else
    head--;

  *head = thisDistance;

  averageDistance = 0;
  for (int i = 0; i < 10; i ++) {
    averageDistance += recentDistances[i];
  }
  averageDistance = averageDistance / 10;

  if (thisDistance < previousAverage + margin ||
      thisDistance > previousAverage - margin)
    return false;
  else
    return true;
}

long SonicSensor::distance()
{
  bool foo;
  long distance, duration;
  do {
    digitalWrite(SS_trig, LOW);
    delayMicroseconds(2);
    digitalWrite(SS_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(SS_trig, LOW);
    duration = pulseIn(SS_echo, HIGH, 80000);
    distance = (duration / 2) / 29.1;
    foo = withinMargin(distance);
  } while (foo);

  return distance;
}


#define CLKpin 13
#define ATT 2
#define DAT 4
#define CMD 5

volatile int CLK = LOW;

void initInterrupt(int freq)
{
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;


  OCR1A = 62500 / freq;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

ISR(TIMER1_COMPA_vect)
{
  if (CLK == 0)
    CLK = 1;
  else if (CLK == 1)
    CLK = 0;
  digitalWrite(CLKpin, CLK);
  //Serial.print("CLK: ");
  //Serial.println(CLK);
}

void setup() {
  pinMode(CLKpin, OUTPUT);
  pinMode(ATT, OUTPUT);
  pinMode(DAT, INPUT);
  pinMode(CMD, OUTPUT);

  digitalWrite(ATT, HIGH);

  initInterrupt(125);
  //Serial.begin(9600);
  //Serial.println("Initializatoin complete...");
  delay(500);
}

void loop() {
  unsigned char command[4] = {0x42, 0x00, 0x00, 0x00};
  unsigned char data[4] = {0};

  //Serial.println("Begining transmition...");
  // Begin Transmition
  digitalWrite(ATT, LOW);
  //Serial.println("BEFORE WHILE");
  while (CLK == 0);
  //Serial.println("AFTER WHILE");
  // CMD = 0x01 LSB!!
  digitalWrite(CMD, 1);
  while (CLK == HIGH);
  while (CLK == LOW);
  digitalWrite(CMD, 0);
  while (CLK);
  while (!CLK);
  digitalWrite(CMD, 0);
  while (CLK);
  while (!CLK);
  digitalWrite(CMD, 0);
  while (CLK);
  while (!CLK);
  digitalWrite(CMD, 0);
  while (CLK);
  while (!CLK);
  digitalWrite(CMD, 0);
  while (CLK);
  while (!CLK);
  digitalWrite(CMD, 0);
  while (CLK);
  while (!CLK);
  digitalWrite(CMD, 0);
  while (CLK);
  while (!CLK);

  // CMD = 0x42 => 0100_0010 and read DATA Byte 1 LSB!!
  digitalWrite(CMD, 0);
  while (CLK);
  data[0] |= (digitalRead(DAT) << 0);
  while (!CLK);
  digitalWrite(CMD, 1);
  while (CLK);
  data[0] |= (digitalRead(DAT) << 1);
  while (!CLK);
  digitalWrite(CMD, 0);
  while (CLK);
  data[0] |= (digitalRead(DAT) << 2);
  while (!CLK);
  digitalWrite(CMD, 0);
  while (CLK);
  data[0] |= (digitalRead(DAT) << 3);
  while (!CLK);
  digitalWrite(CMD, 0);
  while (CLK);
  data[0] |= (digitalRead(DAT) << 4);
  while (!CLK);
  digitalWrite(CMD, 0);
  while (CLK);
  data[0] |= (digitalRead(DAT) << 5);
  while (!CLK);
  digitalWrite(CMD, 1);
  while (CLK);
  data[0] |= (digitalRead(DAT) << 6);
  while (!CLK);
  digitalWrite(CMD, 0);
  while (CLK);
  data[0] |= (digitalRead(DAT) << 7);
  while (!CLK);

  // CMD = idle and read DATA Byte 2 LSB!!
  digitalWrite(CMD, 1);
  while (CLK);
  data[1] |= (digitalRead(DAT) << 0);
  while (!CLK);
  while (CLK);
  data[1] |= (digitalRead(DAT) << 1);
  while (!CLK);
  while (CLK);
  data[1] |= (digitalRead(DAT) << 2);
  while (!CLK);
  while (CLK);
  data[1] |= (digitalRead(DAT) << 3);
  while (!CLK);
  while (CLK);
  data[1] |= (digitalRead(DAT) << 4);
  while (!CLK);
  while (CLK);
  data[1] |= (digitalRead(DAT) << 5);
  while (!CLK);
  while (CLK);
  data[1] |= (digitalRead(DAT) << 6);
  while (!CLK);
  while (CLK);
  data[1] |= (digitalRead(DAT) << 7);
  while (!CLK);

  //CMD == idle and read DATA Byte 3 LSB!!
  while (CLK);
  data[2] |= (digitalRead(DAT) << 0);
  while (!CLK);
  while (CLK);
  data[2] |= (digitalRead(DAT) << 1);
  while (!CLK);
  while (CLK);
  data[2] |= (digitalRead(DAT) << 2);
  while (!CLK);
  while (CLK);
  data[2] |= (digitalRead(DAT) << 3);
  while (!CLK);
  while (CLK);
  data[2] |= (digitalRead(DAT) << 4);
  while (!CLK);
  while (CLK);
  data[2] |= (digitalRead(DAT) << 5);
  while (!CLK);
  while (CLK);
  data[2] |= (digitalRead(DAT) << 6);
  while (!CLK);
  while (CLK);
  data[2] |= (digitalRead(DAT) << 7);
  while (!CLK);

  //CMD == idle and read DATA Byte 4 LSB!!
  while (CLK);
  data[3] |= (digitalRead(DAT) << 0);
  while (!CLK);
  while (CLK);
  data[3] |= (digitalRead(DAT) << 1);
  while (!CLK);
  while (CLK);
  data[3] |= (digitalRead(DAT) << 2);
  while (!CLK);
  while (CLK);
  data[3] |= (digitalRead(DAT) << 3);
  while (!CLK);
  while (CLK);
  data[3] |= (digitalRead(DAT) << 4);
  while (!CLK);
  while (CLK);
  data[3] |= (digitalRead(DAT) << 5);
  while (!CLK);
  while (CLK);
  data[3] |= (digitalRead(DAT) << 6);
  while (!CLK);
  while (CLK);
  data[3] |= (digitalRead(DAT) << 7);

  // Stop transmition
  digitalWrite(ATT, HIGH);

  Serial.begin(9600);
  Serial.println("DATA:");
  Serial.print(+data[0], BIN);
  Serial.print(" - ");
  Serial.print(+data[1], BIN);
  Serial.print(" - ");
  Serial.print(+data[2], BIN);
  Serial.print(" - ");
  Serial.println(+data[3], BIN);
  Serial.end();
  delay(3000);
}




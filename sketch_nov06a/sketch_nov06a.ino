void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  double myArray[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double sum = 0;
  for(int i = 0; i < 10; i++)
  {
    sum += myArray[i];
  }
  sum /= 10;
  Serial.println(sum);
}

 int rangeArray[10];
 int rangeArrayIndex = 0;
 
 void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  while(rangeArrayIndex < 10)
  {
    rangeArray[rangeArrayIndex] = random(100);
    rangeArrayIndex++; 
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(50);

  if(rangeArrayIndex >= 10)
  {
    rangeArrayIndex = 0;
  }
  else
  {
    rangeArrayIndex++;
  }
  rangeArray[rangeArrayIndex] = random(200);

  int filteredRange = 0;
  for(int i = 0; i < 10; i++)
  {
    filteredRange += rangeArray[i];

    Serial.print("\trangeArray[i]: ");
    Serial.println(rangeArray[i]);

    Serial.print("\t\tfilteredRange: ");
    Serial.println(filteredRange);
  }

  Serial.print("Filtered Range Sum: ");
  Serial.println(filteredRange);
  filteredRange /= 10;

  Serial.print("Filtered Range: " );
  Serial.println(filteredRange);
}

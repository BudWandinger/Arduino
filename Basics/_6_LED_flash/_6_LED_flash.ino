  #include <Servo.h>
  
  #define LED1 2
  #define LED2 3
  #define LED3 4
  #define LED4 5
  #define LED5 6
  #define LED6 7

  #define BUTTON1 52
  #define BUTTON2 53

  Servo myservo;
  
void flashAll(){
  //while(not(digitalRead(BUTTON1)||digitalRead(BUTTON2))){
    for(int i = 0; i < 6; i++){
    digitalWrite(i+2, HIGH);
    delay(50);
    digitalWrite(i+2, LOW); 
    }
  //}
  return 0;
}

void flashColors(){
  //while(not(digitalRead(BUTTON1)||digitalRead(BUTTON2))){
    for(int i = 0; i < 3; i++){
      digitalWrite(i+2, HIGH);
      digitalWrite(i+5, HIGH);
      delay(100);
      digitalWrite(i+2, LOW);
      digitalWrite(i+5, LOW);  
    }
  //}
  return 0;
}

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);

  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);

  myservo.attach(9);//attachs the servo on pin 9 to servo object
  myservo.write(0);//back to 0 degrees 
  delay(1000);//wait for a second
}

void loop() {
  while(1){
    if(digitalRead(BUTTON1))
      flashAll();
    if(digitalRead(BUTTON2))
      flashColors();

    myservo.write(10);//back to 0 degrees 
    
  } 
}


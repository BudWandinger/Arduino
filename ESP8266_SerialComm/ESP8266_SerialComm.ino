// Basic serial communication with ESP8266
// Uses serial monitor for communication with ESP8266
//
//  Pins
//  Arduino pin 2 (RX) to ESP8266 TX
//  Arduino pin 3 to voltage divider then to ESP8266 RX
//  Connect GND from the Arduiono to GND on the ESP8266
//  Pull ESP8266 CH_PD HIGH
//
// When a command is entered in to the serial monitor on the computer
// the Arduino will relay it to the ESP8266
//

void setup()
{
  /*
  Serial.begin(9600);     // communication with the host computer

  Serial1.begin(9600);
  //Serial2.begin(9600);

  Serial.println("");
  Serial.println("Remember to to set Both NL & CR in the serial monitor.");
  Serial.println("Ready");
  Serial.println("");
  */
}

void loop()
{
  // Check for communications from both modules
  /*
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  //if (Serial2.available()) {
  //  Serial.write(Serial2.read());
  //}

  if(Serial.available()){
    Serial1.write(Serial.read());
  
  }*/
  /*
  if (Serial.available()) {
    String string = Serial.readString();
    if (string[0] == '1') {
      Serial.println("Sending communications to module 1...");
      for (int i = 1; i < string.length(); i++)
        Serial1.write(string[i]);
    }
    else if (string[0] == '2') {
      Serial.println("Sending communications to module 2...");
      for (int i = 1; i < string.length(); i++)
        Serial2.write(string[i]);
    }
    else
      Serial.println("ERROR: Module number must be entered at beggining of every AT instruction");
  }
  */
  /*if (Serial.available()) {
    if (Serial.peek() == '/') {
      Serial.read();
      if (Serial.peek() == '1') {
        Serial.read();
        MODULE = 1;
        Serial.println("Module 1 selected...");
      }
      else if (Serial.peek() == '2') {
        Serial.read();
        MODULE = 2;
        Serial.println("Module 2 selected...");
      }
    }

    if (MODULE == 1)
      Serial1.write(Serial.read());
    else if (MODULE == 2)
      Serial2.write(Serial.read());

    }*/

  /*if (Serial.available()) {
    if(Serial.readStringUntil(' ') == "1"){
      Serial.println("Module 1 Selected");
      Serial1.write(Serial.read());
    }
    else if(Serial.readStringUntil(' ') == "2"){
      Serial.println("Module 2 Selected");
      Serial2.write(Serial.read());
    }
    else{
      Serial.read();
      Serial.println("Invalid Serial Identifier. Serial buffer cleared.");
    }
    }*/
}

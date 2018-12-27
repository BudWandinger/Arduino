#include <MIDIUSB.h>

void setup() 
{  
  Serial.begin(9600);
}

void loop() 
{
  static int control = 0;
  
  delay(10000);
  
  midiEventPacket_t event = {0x0B, 0xB0 | 0, control, 127};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();

  Serial.println(control);

  if(control == 5)
  {
    control = 0;
  }
  else
  {
    control++;
  }
}

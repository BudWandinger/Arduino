////////////////////////////////////////////////////////////////////////////////
// Module Name:   TwoButtonMidiController
// File Name:     TwoButtonMidiController.ino
// Versoin:       000.000.001
// Created On:    February 4, 2019
// Created By:    Bud Wandinger
// Modified:      February 4, 2019
// Modified By:   Bud Wandinger
// Description:
//    This script uses digital pins 2 and 3 on an arduino micro with pullup resistors on
//    to sense when a button on either pin has been pressed. Upon pressing of a button
//    a related midi command is sent over USB. This script performs button debouncing.
//    This script requires that both digital pins 2 and 3 are connected to ground through
//    seperate buttons.
// Disclaimer:
//    THIS CODE IS PROVIDED “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING THE
//    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//    IN NO EVENT SHALL PAGERDUTY OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//    SUSTAINED BY YOU OR A THIRD PARTY, HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
//    IN CONTRACT, STRICT LIABILITY, OR TORT ARISING IN ANY WAY OUT OF THE USE OF THIS SAMPLE CODE,
//    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////
#include <MIDIUSB.h>


////////////////////////////////////////////////////////////////////////////////
// CONFIGURATION
////////////////////////////////////////////////////////////////////////////////
//#define TIMER_DEBUG
#define MIDI_MESSAGE_DEBUG
#define BUTTON_DEBUG


////////////////////////////////////////////////////////////////////////////////
// PRIVATE DEFINES
////////////////////////////////////////////////////////////////////////////////
#define BUTTON_ONE_PIN 2
#define BUTTON_TWO_PIN 3
#define DEBOUNCING_TIME 25
#if defined(TIMER_DEBUG) || defined(MIDI_MESSAGE_DEBUG) || defined(BUTTON_DEBUG)
  #define SERIAL_BAUD 115200
#endif

enum Button_State {
  BUTTON_RELEASED,
  BUTTON_DEBOUNCING_ON,
  BUTTON_PRESSED,
  BUTTON_DEBOUNCING_OFF
};


////////////////////////////////////////////////////////////////////////////////
// PRIVATE VARIABLES
////////////////////////////////////////////////////////////////////////////////
static uint32_t last_button1_state_change_time;
static uint32_t last_button2_state_change_time;
static Button_State button1_state;
static Button_State button2_state;


////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
////////////////////////////////////////////////////////////////////////////////
static uint32_t calcDeltaTime(uint32_t timeA_, uint32_t timeB_);
static void midiCommandStateChange(byte state_);


////////////////////////////////////////////////////////////////////////////////
// INIT
////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(BUTTON_ONE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_TWO_PIN, INPUT_PULLUP);

  button1_state = BUTTON_RELEASED;
  button2_state = BUTTON_RELEASED;

  #if defined(TIMER_DEBUG) || defined(MIDI_MESSAGE_DEBUG) || defined(BUTTON_DEBUG)
    Serial.begin(115200);
  #endif
}


////////////////////////////////////////////////////////////////////////////////
// MASTER LOOP
////////////////////////////////////////////////////////////////////////////////
void loop()
{ 
  // Button 1 state machine.
  switch (button1_state)
  {
    case BUTTON_RELEASED:
    {
      if(!digitalRead(BUTTON_ONE_PIN))
      {
        last_button1_state_change_time = millis();
        button1_state = BUTTON_DEBOUNCING_ON;
      }
      break;
    }

    case BUTTON_DEBOUNCING_ON:
    {
      if(!digitalRead(BUTTON_ONE_PIN))
      {
        if(calcDeltaTime(last_button1_state_change_time, millis()) > DEBOUNCING_TIME)
        {
           #if defined(BUTTON_DEBUG)
             Serial.println("Button 1 pressed");
           #endif
           midiCommandStateChange((byte)0x01);
           button1_state = BUTTON_PRESSED;
        }
      }
      else
      {
        button1_state = BUTTON_RELEASED;
      }
      break;
    }

    case BUTTON_PRESSED:
    {
      if(digitalRead(BUTTON_ONE_PIN))
      {
        last_button1_state_change_time = millis();
        button1_state = BUTTON_DEBOUNCING_OFF;
      }
      break;
    }

    case BUTTON_DEBOUNCING_OFF:
    {
      if(digitalRead(BUTTON_ONE_PIN))
      {
        if(calcDeltaTime(last_button1_state_change_time, millis()) > DEBOUNCING_TIME)
        {
          #if defined(BUTTON_DEBUG)
            Serial.println("Button1 released");
          #endif
          button1_state = BUTTON_RELEASED;
        }
      }
      else
      {
        button1_state = BUTTON_PRESSED;
      }
      break;
    }
  }

  // Button 2 state machine.
  switch (button2_state)
  {
    case BUTTON_RELEASED:
    {
      if(!digitalRead(BUTTON_TWO_PIN))
      {
        last_button2_state_change_time = millis();
        button2_state = BUTTON_DEBOUNCING_ON;
      }
      break;
    }

    case BUTTON_DEBOUNCING_ON:
    {
      if(!digitalRead(BUTTON_TWO_PIN))
      {
        if(calcDeltaTime(last_button2_state_change_time, millis()) > DEBOUNCING_TIME)
        {
           #if defined(BUTTON_DEBUG)
             Serial.println("Button 2 pressed");
           #endif
           midiCommandStateChange((byte)0x02);
           button2_state = BUTTON_PRESSED;
        }
      }
      else
      {
        button2_state = BUTTON_RELEASED;
      }
      break;
    }

    case BUTTON_PRESSED:
    {
      if(digitalRead(BUTTON_TWO_PIN))
      {
        last_button2_state_change_time = millis();
        button2_state = BUTTON_DEBOUNCING_OFF;
      }
      break;
    }

    case BUTTON_DEBOUNCING_OFF:
    {
      if(digitalRead(BUTTON_TWO_PIN))
      {
        if(calcDeltaTime(last_button2_state_change_time, millis()) > DEBOUNCING_TIME)
        {
          #if defined(BUTTON_DEBUG)
            Serial.println("Button2 released");
          #endif
          button2_state = BUTTON_RELEASED;
        }
      }
      else
      {
        button2_state = BUTTON_PRESSED;
      }
      break;
    }
  }
}


////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS 
////////////////////////////////////////////////////////////////////////////////
/*
 * Calculates and returns the diffence in milliseconds between timeA_ and timeB_: timeB_ - timeA_.
 */
static uint32_t calcDeltaTime(uint32_t timeA_, uint32_t timeB_)
{
  uint32_t deltaTime;

  if (timeB_ >= timeA_)
  {
    deltaTime = timeB_ - timeA_;
  }
  else
  {
    deltaTime = timeB_ + (0xFFFFFFFFFFFFFFFF - timeB_);
  }

#if defined(TIMER_DEBUG)
  Serial.print("TIMER: timeB_: ");
  Serial.print(timeB_);
  Serial.print("  -  timeA_: ");
  Serial.print(timeA_);
  Serial.print("  =  delta time: ");
  Serial.println(deltaTime);
#endif

  return deltaTime;
}

/*
 * Sends midi commands and updates the midi message time tracking variable to track at what time the
 * last Midi message was sent.
 */
static void midiCommandStateChange(byte state_)
{
  /* Send the new MIDI message */
  midiEventPacket_t event = {0x0B, 0xB0 | 0, state_, 127};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();

#if defined(MIDI_MESSAGE_DEBUG)
  Serial.print("**** MIDI MESSAGE SENT: ");
  Serial.print(state_);
  Serial.println(" *****");
#endif
}

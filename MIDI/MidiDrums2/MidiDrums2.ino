/* MIDI Drums v2

*/

#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
#include <SoftwareSerial.h>
#include "pad.h"

// configuration:
const int pinPoti = A0;
const int pinMidiIn = 2;
const int pinMidiOut = 3;
const int pinPiezo1 = A2;
const int pinPiezo2 = A3;
const int pinLed = 13;

const int midiChan = 10;
const int noteValue = 32;
const long noteDuration = 50; //ms
const long velo_mapmax = 1023;

int ledState = LOW;
unsigned long startMillis = 0;
unsigned long currentMillis = 0;
int noteVelocity = 0;

SoftwareSerial midiSerial(pinMidiIn, pinMidiOut); //Midi Port
MIDI_CREATE_INSTANCE(SoftwareSerial, midiSerial, MIDI);

/*Piezo piezo1(pinPiezo1);*/
Pad pads[2];

void setup()
{
  //setup pads
  pads[0] = Pad(pinPiezo1, noteValue, midiChan); 
  pads[1] = Pad(pinPiezo2, noteValue+2, midiChan);
  
  //pinMode(pinLed, OUTPUT); // declare the ledPin as as OUTPUT
  //Serial.begin(9600);       // use the serial port
}

void loop()
{
  for(int i=0; i<2; i++)
  {
    if(pads[i].process())
    {
      int vel = pads[i].getVelocity();
      if(vel>0) MIDI.sendNoteOn (pad[i].getNote(), vel, pad[i].getChannel());
      else      MIDI.sendNoteOff(pad[i].getNote(), 0,   pad[i].getChannel());
    }
  }
  
  // query piezo1 sensor
  /*if (piezo1.isHit()) {
    // toggle the status of the ledPin:
    ledState = !ledState;
    // update the LED pin itself:
    digitalWrite(pinLed, ledState);

    int potiRaw = analogRead(pinPoti);
    Serial.print("Poti "); Serial.println(potiRaw);

    startMillis = millis();
    noteVelocity = map(piezo1.getRawValue(), 0, potiRaw, 0, 127);
    if (noteVelocity > 127) noteVelocity = 127;
    Serial.print("Note On "); Serial.print(noteValue); Serial.print(" "); Serial.println(noteVelocity);
    MIDI.sendNoteOn(noteValue, noteVelocity, midiChan);
  }

  currentMillis = millis();
  if (noteVelocity > 0 && currentMillis - startMillis >= noteDuration) {

    MIDI.sendNoteOff(noteValue, 0, midiChan);
    Serial.println("Note Off");
    noteVelocity = 0;
  }*/

  //delay(100);  // delay to avoid overloading the serial port buffer
}


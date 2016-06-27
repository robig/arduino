/* MIDI Drums v2

*/

#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>

//#define DEBUG

#include "pad.h"
#include "blinker.h"
#include "switch.h"

// configuration:
const int pinPoti = A0;
const int pinMidiIn = 2;
const int pinMidiOut = 3;
const int pinPiezo1 = A2;
const int pinPiezo2 = A3;
const int pinLed = 13;
const int pinButton = 10;

const int midiChan = 10;
const int noteValue = 32;
const long noteDuration = 50; //ms
const long velo_mapmax = 1023;

int calibrationMode = LOW; //remove me

int noteVelocity = 0;
int maxRawValue = 1;

#ifdef DEBUG
#include <SoftwareSerial.h>
SoftwareSerial midiSerial(pinMidiIn, pinMidiOut); //Midi Port
MIDI_CREATE_INSTANCE(SoftwareSerial, midiSerial, MIDI);
#endif
#ifndef DEBUG
MIDI_CREATE_DEFAULT_INSTANCE();
#endif

/*Piezo piezo1(pinPiezo1);*/
#define PADS 2
Pad pads[PADS];
Blinker modeLed(pinLed);
Switch modeSw(pinButton);

void setup()
{
  //setup pads
  pads[0] = Pad(pinPiezo1, noteValue, midiChan); 
  pads[1] = Pad(pinPiezo2, noteValue+2, midiChan);
  
  //pinMode(pinLed, OUTPUT); // declare the ledPin as as OUTPUT
  #ifdef DEBUG
    Serial.begin(9600);
    pads[0].enableLogging(true);
  #endif
}

void loop()
{
  /***** Piezo Pad logic: *****/
  for(int i=0; i<PADS; i++)
  {
    if(pads[i].process())
    {
      noteVelocity = pads[i].getVelocity();
      if(noteVelocity>0) MIDI.sendNoteOn (pads[i].getNote(), noteVelocity, pads[i].getChannel());
      else               MIDI.sendNoteOff(pads[i].getNote(), 0,            pads[i].getChannel());
      
      if(noteVelocity>0 && modeSw.isHIGH()) //calibration
      {
        //maxRawValue = max(maxRawValue, pads[i].getPiezo()->getRawValue());
        if(pads[i].getPiezo()->getRawValue() > maxRawValue)
        {
          maxRawValue = pads[i].getPiezo()->getRawValue();
          #ifdef DEBUG
          Serial.print("cal: "); Serial.println(maxRawValue);
          #endif
          pads[0].setMapMaxValue(maxRawValue);
        }
      }
    }
  }
  /****** end pads *****/

  // modeLed blinker
  modeSw.process();
  modeLed.setEnabled(modeSw.isHIGH());
  modeLed.process();
  if(modeSw.isHIGH() && modeSw.stateChanged()) maxRawValue = 1; //reset to minimum value
  

  if(calibrationMode == 3) //poti disabled for now
  {
    int potiRaw = analogRead(pinPoti);
    /** for controlling trigger threshold with poti **/
    /*float thres = pads[0].getPiezo()->getThreshold();
    pads[0].getPiezo()->setThresholdRaw(potiRaw);
    #ifdef DEBUG
    if(abs(pads[0].getPiezo()->getThreshold() - thres) > 0.01)
    {
    Serial.print("Poti "); Serial.println(pads[0].getPiezo()->getThreshold());
    }
    #endif
    */
  
    
    /** for controlling velocity mapping with poti **/
    #ifdef DEBUG
    if(abs(pads[0].getMapMaxValue() - potiRaw) > 3)
    {
    Serial.print("Poti "); Serial.println(potiRaw);
    }
    #endif
    pads[0].setMapMaxValue(potiRaw);
  }
}


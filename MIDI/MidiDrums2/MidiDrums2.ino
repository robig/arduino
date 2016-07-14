/* MIDI Drums v2

*/
//MIDI note defines for each trigger
#define SNARE_NOTE 38
#define LTOM_NOTE 40
#define RTOM_NOTE 72
#define LCYM_NOTE 73
#define RCYM_NOTE 74
#define KICK_NOTE 36

#define MIN_NOTE 35
#define MAX_NOTE 81

//MIDI defines
#define NOTE_ON_CMD 0x90
#define NOTE_OFF_CMD 0x80
#define MAX_MIDI_VELOCITY 127

//MIDI baud rate
#define SERIAL_RATE 31250

#define DEBUG
//#define DEBUG_NOMIDI
#define LOGLEVEL LOG_LEVEL_DEBUG

#include "logger.h"
#include "pad.h"
#include "blinker.h"
#include "switch.h"
#include "button.h"

// configuration:
const int pinPoti   = A0;
const int pinDbgRx  = 10;
const int pinDbgTx  = 11;
const int pinPiezo1 = A2;
const int pinPiezo2 = A3;
const int pinPiezo3 = A4;
const int pinLed1   = 7;
const int pinLed2   = 8;
const int pinLed3   = 9;
const int pinModeBtn = 2;

const int pinNoteButtonNext  = 3;
const int pinNoteButtonRaise = 4;
const int pinNoteButtonLower = 5;

const int midiChan = 10;
const long noteDuration = 50; //ms
const int minInterval = 100; //ms
const int maxInterval = 1000;

int calibrationMode = LOW; //remove me

int noteVelocity = 0;
int maxRawValue = 1;

#ifdef DEBUG_NOMIDI
//TODO MIDI_CREATE_INSTANCE(SoftwareSerial, dbgSerial, MIDI);
#endif

// number of drum pads
#define PADS 3

Pad pads[PADS];
Blinker leds[PADS];
Switch modeSw(pinModeBtn);

//pad selector
unsigned short selectedPad = 0;

//coltrol buttons
Button buttonNext(pinNoteButtonNext);
Button buttonRaise(pinNoteButtonRaise);
Button buttonLower(pinNoteButtonLower);

void setup()
{
  Serial.begin(SERIAL_RATE);
  
  //setup pads
  pads[0] = Pad(pinPiezo1, SNARE_NOTE, 0); 
  pads[1] = Pad(pinPiezo2, KICK_NOTE,  1);
  pads[2] = Pad(pinPiezo3, LTOM_NOTE,  2);

  //setup leds
  leds[0] = Blinker(pinLed1);
  leds[1] = Blinker(pinLed2);
  leds[2] = Blinker(pinLed3);
  
  //pinMode(pinLed, OUTPUT); // declare the ledPin as as OUTPUT
  #ifdef DEBUG
    Log.Init(LOGLEVEL, 9600, pinDbgRx, pinDbgTx);
    Log.Info("Inizialized and ready");
    pads[0].enableLogging(true);
    pads[1].enableLogging(true);
    pads[2].enableLogging(true);
  #endif
}

void loop()
{
  unsigned short i=0;
  /***** Drum Pad logic: *****/
  for(i=0; i<PADS; i++)
  {
    if(pads[i].process())
    {
      noteVelocity = pads[i].getVelocity();
      Log.Debug("#%i velocity=%i", i, noteVelocity);
      if(noteVelocity>0)
      {
        //play a note
        midiNoteOn (pads[i].getNote(), noteVelocity); //todo , pads[i].getChannel());
        midiNoteOff(pads[i].getNote(), 0           ); //  pads[i].getChannel());
      }
      
      if(modeSw.isHIGH()) //calibration
      {
        Log.Debug("pad=%i i=%i pads=%i", selectedPad, i, PADS);
        //maxRawValue = max(maxRawValue, pads[i].getPiezo()->getRawValue());
        //if(pads[i].getPiezo()->getRawValue() > maxRawValue)
        if(selectedPad == i) //nur aktuelles pad einstellen
        {
          int raw = pads[i].getRawValue();
          int ms = map(127 - noteVelocity, 0, 127, minInterval, maxInterval);
          #ifdef DEBUG
          Log.Info("cal: pad=%i max=%i ms=%i", selectedPad, raw, ms);
          #endif
          pads[selectedPad].setMapMaxValue(raw);
          
        }
      }
    }
  }
  /****** end pads *****/

  /** calibration mode stuff **/
  // mode switch
  modeSw.process();
  for(i=0; i<PADS; i++)
  {
    leds[i].process(); //led blinking
    leds[i].setEnabled(modeSw.isHIGH() && selectedPad == i);
  }
  if(modeSw.isHIGH() && modeSw.stateChanged())
  {
    Log.Info("MODE active. calibrate now. active pad: %i", selectedPad);
    maxRawValue = 1; //reset to minimum value
  }

  /** switch instrument **/
  if(buttonNext.pressed())
  {
    selectedPad++;
    if(selectedPad >= PADS) selectedPad = 0;
    #ifdef DEBUG
    Log.Debug("selected Pad %i", selectedPad);
    #endif
  }
  if(!modeSw.isHIGH())
  {
    if(buttonRaise.pressed())
    {
      int note = pads[selectedPad].getNote() + 1;
      if(note > MAX_NOTE) note = MIN_NOTE;
      pads[selectedPad].setNote(note);
      #ifdef DEBUG
      Log.Debug(" Pad %i playes note %i", selectedPad, note);
      #endif
    }
    if(buttonLower.pressed())
    {
      int note = pads[selectedPad].getNote() - 1;
      if(note < MIN_NOTE) note = MAX_NOTE;
      pads[selectedPad].setNote(note);
      #ifdef DEBUG
      Log.Debug(" Pad %i playes note %i", selectedPad, note);
      #endif
    }
  }
  else
  {
    if(buttonRaise.pressed())
    {
      int thr = pads[selectedPad].getPiezo()->getThreshold() + 1;
      if(thr > 1023) thr = 1;
      pads[selectedPad].getPiezo()->setThreshold(thr);
      #ifdef DEBUG
      Log.Debug(" Pad %i set threshold=%i", selectedPad, thr);
      #endif
    }
    if(buttonLower.pressed())
    {
      int thr = pads[selectedPad].getPiezo()->getThreshold() - 1;
      if(thr <= 1) thr = 1020;
      pads[selectedPad].getPiezo()->setThreshold(thr);
      #ifdef DEBUG
      Log.Debug(" Pad %i set threshold=%i", selectedPad, thr);
      #endif
    }
  }
}

void noteFire(unsigned short note, unsigned short velocity)
{ 
  midiNoteOn(note, velocity);
  midiNoteOff(note, velocity);
}

void midiNoteOn(byte note, byte velocity)
{
  if(velocity > MAX_MIDI_VELOCITY)
    velocity = MAX_MIDI_VELOCITY;
  Serial.write(NOTE_ON_CMD);
  Serial.write(note);
  Serial.write(velocity);
}

void midiNoteOff(byte note, byte velocity)
{
  if(velocity > MAX_MIDI_VELOCITY)
    velocity = MAX_MIDI_VELOCITY;
  Serial.write(NOTE_OFF_CMD);
  Serial.write(note);
  Serial.write(velocity);
}


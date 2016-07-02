/* MIDI Drums v2

*/
//MIDI note defines for each trigger
#define SNARE_NOTE 38
#define LTOM_NOTE 71
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
const int pinLed    = 8;
const int pinButton = 2;

const int pinNoteButtonNext  = 3;
const int pinNoteButtonRaise = 4;
const int pinNoteButtonLower = 5;

const int midiChan = 10;
const long noteDuration = 50; //ms

int calibrationMode = LOW; //remove me

int noteVelocity = 0;
int maxRawValue = 1;

#ifdef DEBUG_NOMIDI
//TODO MIDI_CREATE_INSTANCE(SoftwareSerial, dbgSerial, MIDI);
#endif

// number of drum pads
#define PADS 2

Pad pads[PADS];
Blinker modeLed(pinLed);
Switch modeSw(pinButton);

//instrument selection
unsigned short selectedPad = 0;
Button buttonNext(pinNoteButtonNext);
Button buttonRaise(pinNoteButtonRaise);
Button buttonLower(pinNoteButtonLower);

void setup()
{
  Serial.begin(SERIAL_RATE);
  
  //setup pads
  pads[0] = Pad(pinPiezo1, SNARE_NOTE, midiChan); 
  pads[1] = Pad(pinPiezo2, KICK_NOTE,  midiChan);
  
  //pinMode(pinLed, OUTPUT); // declare the ledPin as as OUTPUT
  #ifdef DEBUG
    Log.Init(LOGLEVEL, 9600, pinDbgRx, pinDbgTx);
    Log.Info("Inizialized and ready");
    pads[0].enableLogging(true);
    pads[1].enableLogging(true);
  #endif
}

void loop()
{
  /***** Drum Pad logic: *****/
  for(int i=0; i<PADS; i++)
  {
    if(pads[i].process())
    {
      noteVelocity = pads[i].getVelocity();
      //Log.Debug("#%i velocity=%i", i, noteVelocity);
      if(noteVelocity>0)
      {
        midiNoteOn (pads[i].getNote(), noteVelocity); //todo , pads[i].getChannel());
        midiNoteOff(pads[i].getNote(), 0           ); //  pads[i].getChannel());
      }
      
      if(noteVelocity>0 && modeSw.isHIGH()) //calibration
      {
        //maxRawValue = max(maxRawValue, pads[i].getPiezo()->getRawValue());
        if(pads[i].getPiezo()->getRawValue() > maxRawValue)
        {
          maxRawValue = pads[i].getPiezo()->getRawValue();
          #ifdef DEBUG
          Log.Info("cal: %i", maxRawValue);
          #endif
          pads[0].setMapMaxValue(maxRawValue);
        }
      }
    }
  }
  /****** end pads *****/

  /** calibration mode stuff **/
  // modeLed blinker
  modeSw.process();
  modeLed.setEnabled(modeSw.isHIGH());
  modeLed.process();
  if(modeSw.isHIGH() && modeSw.stateChanged())
  {
    Log.Info("MODE active. calibrate now.");
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


#include "Arduino.h"
#include "pad.h"

Pad::Pad(){
  
}

Pad::Pad(int pin, int note, int chan)
{
  _pin = pin;
  _note = note;
  _channel = chan;
  //_midi = midi;
  _piezo = new Piezo(_pin);
  
  _release = 50; // default release time
  _startMillis = 0;
  _noteVelocity = 0;
  _mapmax = 173; //Per poti ermittelt

  /*Serial.begin(9600);*/
  _logging=false;
}

    
boolean Pad::process()
{
  // query piezo sensor
  if (_piezo->isHit()) {
    _startMillis = millis();
    _noteVelocity = map(_piezo->getRawValue(), 0, _mapmax, 0, 127);
    if (_noteVelocity > 127) _noteVelocity = 127;
    if(_logging) { Serial.print("Note On "); Serial.print(_note); Serial.print(" "); Serial.println(_noteVelocity); }
    //_midi.sendNoteOn(_note, _noteVelocity, _channel);
    return true;
  }

  unsigned long now = millis();
  if (_noteVelocity > 0 && now - _startMillis >= _release) {
    //_midi.sendNoteOff(_note, 0, _channel);
    if(_logging) Serial.println("Note Off");
    _noteVelocity = 0;
    return true;
  }
  return false;
}

int Pad::getNote()
{
  return _note;
}

int Pad::getChannel()
{
  return _channel;
}

int Pad::getVelocity()
{
  return _noteVelocity;
}

Piezo *Pad::getPiezo()
{
  return _piezo;
}

void Pad::enableLogging(boolean dbg)
{
  _logging = dbg;
}

void Pad::setRelease(int releaseTime)
{
  if(releaseTime < 0 || releaseTime > 1000) return;
  _release = releaseTime;
}

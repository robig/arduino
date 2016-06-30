#include "Arduino.h"
#include "pad.h"

#define SIGNAL_BUFFER_SIZE 100

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

  _logging=false;

  // setup buffer:
  _bufferMax = 0;
}

    
boolean Pad::process()
{
  boolean ret = false;
  // query piezo sensor
  unsigned short newSignal = _piezo->getRawValue();
  //_buffer[_bufferIndex] = newSignal;
  if(newSignal > _bufferMax)
  {
    _bufferMax = newSignal;
    _startMillis = millis();
  }
  
  if (!_piezo->isHit()) //value < threshold, so we maybe got a peak lately
  {
    _noteVelocity = mapRawValue(_bufferMax);
    if (_noteVelocity > 127) _noteVelocity = 127;
    if(_logging) { Log.Debug("Note On %i %i %i", _pin, _note, _noteVelocity); }
    resetBuffer();
    ret = true;
  }

  _bufferIndex++;
  if(_bufferIndex == SIGNAL_BUFFER_SIZE) _bufferIndex = 0;
  return ret;
}

void Pad::resetBuffer()
{
  _bufferMax = 0;
  _bufferIndex = 0;
}

int Pad::mapRawValue(int raw)
{
  return map(raw, 0, _mapmax, 0, 127);
}

void Pad::setMapMaxValue(int raw)
{
  _mapmax = raw;
}

int Pad::getMapMaxValue()
{
  return _mapmax;
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

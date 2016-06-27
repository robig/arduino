#include "Arduino.h"
#include "switch.h"

Switch::Switch(int pin)
{
  _pin = pin;
  _release = 200;
  _state = LOW;
  _value = LOW;
  _lastValue = LOW;
  pinMode(_pin, INPUT);
}

void Switch::process()
{
  unsigned long currentMillis = millis();
  _statechange = false;
  
  if (currentMillis - _previousMillis >= _release) {
    _previousMillis = currentMillis;

    // read Button 
    _value = digitalRead(_pin);
    if(_value == LOW && _value != _lastValue)
    {
      _state=!_state;
      _statechange = true;
      #ifdef DEBUG
      Serial.print("Mode="); Serial.println(_state);
      #endif
      //modeLed.setEnabled(calibrationMode == HIGH);
    }
    _lastValue = _value;
  }
}

int Switch::getState()
{
  return _state;
}

boolean Switch::stateChanged()
{
  return _statechange;
}

boolean Switch::isHIGH()
{
  return _state == HIGH;
}


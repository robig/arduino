
#include "Arduino.h"
#include "button.h"

Button::Button(int pin)
{
  _pin=pin;
  _lastButtonState = LOW;
  _lastDebounceTime = 0;
  _debounceDelay = 50;  
}

boolean Button::pressed()
{
  boolean ret=false;
  // read the state of the switch into a local variable:
  int reading = digitalRead(_pin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != _lastButtonState) {
    // reset the debouncing timer
    _lastDebounceTime = millis();
  }

  if ((millis() - _lastDebounceTime) > _debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != _buttonState) {
      _buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (_buttonState == HIGH) {
        ret = true;
      }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  _lastButtonState = reading;

  return ret;
}

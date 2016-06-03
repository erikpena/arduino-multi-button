#include "Arduino.h"
#include "MultiButton.h"
#include "ButtonAction.h"

int MultiButton::_pin;
volatile int MultiButton::_state;
volatile long MultiButton::_lastDebounceTime = 0L;
const long MultiButton::_continualDownDelay = 250L;
volatile long MultiButton::_lastButtonDownTime = 0L;
volatile long MultiButton::_buttonDownTime = -1L;
const long MultiButton::_longPressDelay = 4000L;
const int MultiButton::_debounceDelay = 50;

ButtonAction MultiButton::action = NONE;

typedef void (*handleClick)();
typedef void (*handleDoubleClick)();
typedef void (*handleLongClick)();

handleClick _handleClick;
handleDoubleClick _handleDoubleClick;
handleLongClick _handleLongClick;

MultiButton::MultiButton(int pin) {
  _pin = pin;

  // Configure the pin mode as an input.
  pinMode(_pin, INPUT);

  // Attach an interrupt to the pin, assign the onChange function as a handler and trigger on changes (LOW or HIGH).
  attachInterrupt(_pin, onChange, CHANGE);

  action = NONE;
}

void MultiButton::check() {
  if(millis() - _lastButtonDownTime <= _continualDownDelay) return;
  
  if(action == NONE) return;
  else if(action == SINGLE) _handleClick();
  else if(action == DOUBLE) _handleDoubleClick();
  else if(action == LONG) _handleLongClick();

  action = NONE;
}

// Gets called by the interrupt.
void MultiButton::onChange() {
  // Get the pin reading.
  int reading = digitalRead(_pin);

  // Ignore dupe readings.
  if (reading == _state) return;

  boolean debounce = false;

  // Check to see if the change is within a debounce delay threshold.
  if ((millis() - _lastDebounceTime) <= _debounceDelay) {
    debounce = true;
  }

  long now = millis();

  // This update to the last debounce check is necessary regardless of debounce state.
  _lastDebounceTime = now;

  // Ignore reads within a debounce delay threshold.
  if (debounce) return;

  // All is good, persist the reading as the state.
  _state = reading;

  // Work with the value now.
  Serial.println("button state: " + String(_state == 0 ? "down" : "up"));

  // If button is down.
  if (_state == 0) {
    _buttonDownTime = now;
  }
  // If button is up.
  else {
    if(_buttonDownTime == -1L) {
      Serial.println("Last button down time is unset, ignoring");

      return;
    }

    Serial.println("Button down time: " + String(_buttonDownTime));
    Serial.println("Now: " + String(now));
    Serial.println("Down Up Delta: " + String(now - _buttonDownTime));

    if (now - _buttonDownTime >= _longPressDelay) {
      action = LONG;
    } else if(_lastButtonDownTime != -1 && now - _lastButtonDownTime < _continualDownDelay) {
      action = DOUBLE;
    } else {
      action = SINGLE;
    }

    _lastButtonDownTime = now;

    _buttonDownTime = -1L;
  }
}

void MultiButton::setOnClick(handleClick f) {
  _handleClick = f;
}

void MultiButton::setOnDoubleClick(handleDoubleClick f) {
  _handleDoubleClick = f;
}

void MultiButton::setOnLongClick(handleLongClick f) {
  _handleLongClick = f;
}

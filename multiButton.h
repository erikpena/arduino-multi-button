#ifndef MultiButton_h
#define MultiButton_h

#include "Arduino.h"
#include "ButtonAction.h"

class MultiButton {
  public:
    MultiButton(int pin);
    
    // This value gets set as part of the constructor execution.
    static int _pin;
    
    // Holds the current button state.
    static volatile int _state;
    
    // Holds the last time debounce was evaluated (in millis).
    static volatile long _lastDebounceTime;
    
    // The delay threshold for debounce checking.
    static const int _debounceDelay;

    static const long _continualDownDelay;

    // The last time the button was in a down state.
    static volatile long _lastButtonDownTime;
  
    // Holds the millis of when the button was pressed down.
    static volatile long _buttonDownTime;
  
    // Defines the duration in which is held to be interpreted as a long press.
    static const long _longPressDelay;

    static ButtonAction action;

    void check();

    static void setOnClick(void (*handleClick)());

    static void setOnDoubleClick(void (*handleDoubleClick)());

    static void setOnLongClick(void (*handleLongClick)());
  private:  
    static void onChange();
};

#endif

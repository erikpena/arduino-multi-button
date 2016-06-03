# arduino-multi-button
A library for adding multi-functionality to buttons.

This project was created out of necessity because I was doing development for the ESP8266 module where I uploaded my sketches directly to the module and did my development within the Arduino IDE.  Since the ESP8266 (ESP-01) has limited GPIO pins (Two.  One of which I was using as the programming pin), I needed to be smart about how I utilized the pins.  I wanted a button, but I wanted this button to be more than just the standard single press button.  I wanted a button that would allow for differnt interactions.

## Features
* Button press debouncing
* Definable pin assignment
* Uses interrupts to detect a change to button state
* Multi-functional pressing
  * Single press
  * Double press
  * Long press
* Definable callback methods for each type of button press event

## Usage
Below is an example sketch that demonstrates how to use MultiButton.

```c
#include "multiButton.h"

// The GPIO pin number for the button.
const int multiButtonPin = 2;

// Instantiate a new instance of MultiButton.
MultiButton multiButton(multiButtonPin);

void setup() {
  Serial.begin(115200);

  // Set the necessary callback methods per button press event.
  multiButton.setOnClick(&handleClick);
  multiButton.setOnDoubleClick(&handleDoubleClick);
  multiButton.setOnLongClick(&handleLongClick);
}

void loop() {
  // Required in order to fire button press events.
  multiButton.check();
}

void handleClick() {
  Serial.println("handleClick fired");
}

void handleDoubleClick() {
  Serial.println("handleDoubleClick fired");
}

void handleLongClick() {
  Serial.println("handleLongClick fired");
}
```

/*
  SenseoLed.cpp - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#include "SenseoLed.h"

SenseoLed::SenseoLed(int ledPin)
{
  ocSenseLedPin = ledPin;
}


void SenseoLed::setLedChangedAt(unsigned long ms) {
  prevLedChangeMillis = ledChangeMillis;
  ledChangeMillis = ms;
  ledChanged = true;
}


int SenseoLed::getLastPulseDuration() {
  return ledChangeMillis - prevLedChangeMillis;
}


void SenseoLed::updateState() {
  if (ledChanged) {
    // When there was an interrupt from the Senseo LED pin
    int pulseDuration = ledChangeMillis - prevLedChangeMillis;
    // decide if LED is blinking fast or slow
    if (abs(pulseDuration - pulseDurLedFast) < pulseDurTolerance) {
        ledStatePrev = ledState;
        ledState = LED_FAST;
    } else if (abs(pulseDuration - pulseDurLedSlow) < pulseDurTolerance) {
        ledStatePrev = ledState;
        ledState = LED_SLOW;
    } else {
      // Nothing to do here.
      // pulseDuration could be below (user interaction) or above (end of a continuous state) the known times. 
      // No actions needed.
    }
    ledChanged = false;
  }
  // decide if LED is not blinking but in a continuous state
  int t = millis() - ledChangeMillis;
  if (( t > pulseContThreshold) && (t < 2 * pulseContThreshold)) {
    ledStatePrev = ledState;
    ledState = !digitalRead(ocSenseLedPin) ? LED_ON : LED_OFF;
  }
}

bool SenseoLed::hasChanged() {
  // did the LED state change during last updateState() execution? 
  return (ledStatePrev != ledState);
}

ledStateEnum SenseoLed::getState() {
  return ledState;
}

String SenseoLed::getStateAsString() {
  if (ledState == LED_OFF) return "LED_OFF";
  else if (ledState == LED_SLOW) return "LED_SLOW";
  else if (ledState == LED_FAST) return "LED_FAST";
  else if (ledState == LED_ON) return "LED_ON";
  else return "LED_unknown";
}


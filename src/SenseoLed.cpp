/*
  SenseoLed.cpp - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#include "SenseoLed.h"

SenseoLed::SenseoLed(int ledPin) {
  ocSenseLedPin = ledPin;
  ledChangeMillis = millis();
}

void SenseoLed::pinStateToggled() {
  unsigned long now = millis();
  if (now - ledChangeMillis <= LedIgnoreChangeDuration) return; // simple debouncer
  prevLedChangeMillis = ledChangeMillis;
  ledChangeMillis = now;
  ledChanged = true;
}

int SenseoLed::getLastPulseDuration() {
  return (ledChangeMillis - prevLedChangeMillis);
}

void SenseoLed::updateState() {
  ledStatePrev = ledState;
  if (ledChanged) {
    // When there was an interrupt from the Senseo LED pin
    int pulseDuration = ledChangeMillis - prevLedChangeMillis;
    if (true) Homie.getLogger() << "LED observer, last pulse duration: " << pulseDuration << endl;
    // decide if LED is blinking fast or slow
    if (abs(pulseDuration - pulseDurLedFast) < pulseDurTolerance) {
        ledState = LED_FAST;
    } else if (abs(pulseDuration - pulseDurLedSlow) < pulseDurTolerance) {
        ledState = LED_SLOW;
    } else {
      // Nothing to do here.
      // pulseDuration could be below (user interaction) or above (end of a continuous state) the known times.
      // No actions needed.
    }
    ledChanged = false;
  }
  // decide if LED is not blinking but in a continuous state
  int t = (unsigned long)(millis() - ledChangeMillis);
  if (( t > pulseContThreshold) && (t < 3 * pulseContThreshold)) {
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

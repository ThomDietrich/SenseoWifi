/*
Cup.cpp - Library for the SenseoWifi project.
Created by Thomas Dietrich, 2016-03-05.
Released under some license.
*/

#include "Cup.h"

Cup::Cup(int pin)
{
  detectorPin = pin;
  cupAvailable = !digitalRead(detectorPin);
}

void Cup::updateState() {
  bool value = !digitalRead(detectorPin);
  //debounce
  if (value != lastChangeValue) {
    lastChangeValue = value;
    lastChangeMillis = millis();
  }
  if (millis() - lastChangeMillis <= CupDebounceInterval) return;
  // process debounced detector reading
  if (value != cupAvailable) {
    cupAvailable = value;
    cupFilling = false;
    availableChanged = true;
    if (cupAvailable || (!cupAvailable && cupFull)) {
      cupFull = false;
      fullChanged = true;
    }
  }
}

void Cup::setFilling() {
  cupFilling = true;
  cupFull = false;
}

void Cup::setFull() {
  cupFilling = false;
  cupFull = true;
  fullChanged = true;
}

bool Cup::isAvailableChanged() {
  if (availableChanged) {
    availableChanged = false;
    return true;
  };
  return false;
}

bool Cup::isFullChanged() {
  if (fullChanged) {
    fullChanged = false;
    return true;
  };
  return false;
}

bool Cup::isAvailable() {
  return cupAvailable;
}

bool Cup::isNotAvailable() {
  return !cupAvailable;
}

bool Cup::isFilling() {
  return cupFilling;
}

bool Cup::isFull() {
  return cupFull;
}

bool Cup::isEmpty() {
  return !cupFull;
}

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
  debouncer = Bounce();
}

void Cup::initDebouncer() {
  debouncer.attach(detectorPin);
  debouncer.interval(50);
}

void Cup::updateState() {
  debouncer.update();
  bool val = !debouncer.read();
  if (val != cupAvailable) {
    cupAvailable = val;
    availableChanged = true;
    if (cupAvailable == false) {
      cupFull = false;
      fullChanged = true;
    }
  }
}

void Cup::fillUp() {
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

bool Cup::isFull() {
  return cupFull;
}

bool Cup::isEmpty() {
  return !cupFull;
}

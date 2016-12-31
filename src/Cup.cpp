/*
  Cup.cpp - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#include "Cup.h"

Cup::Cup(int pin)
{
  detectorPin = pin;
  debouncer = Bounce();
  updateState();
}

void Cup::initDebouncer() {
  debouncer.attach(detectorPin);
  debouncer.interval(50);
}

void Cup::updateState() {
  changed = false;
  debouncer.update();
  bool val = !debouncer.read();
  if (val != cupAvailable) {
    cupAvailable = val;
    Serial.println(cupAvailable);
    changed = true;
  }
  if (cupAvailable == false) cupFull = false;
}

void Cup::fillUp() {
  if (cupAvailable == true) cupFull = true;
}

bool Cup::hasChanged() {
  return changed;
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

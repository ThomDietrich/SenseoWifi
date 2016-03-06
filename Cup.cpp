/*
  Cup.cpp - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#include "Cup.h"

Cup::Cup(int dPin)
{
  detectorPin = dPin;
  cupAvailable = readDetectorPin();
}

Cup::Cup(int dPin, int bPin) {
  detectorPin = dPin;
  cupAvailable = readDetectorPin();
  beeperPin = bPin;
}


void Cup::updateState() {
  cupAvailable = readDetectorPin();
  if (cupAvailable && (beeperPin != -1)) {
    tone(beeperPin, 512, 100);
  }
}


bool Cup::readDetectorPin() {
  return !digitalRead(detectorPin);
}


bool Cup::isAvailable() {
  //TODO: wait for state to settle
  return cupAvailable;
}

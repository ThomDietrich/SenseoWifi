/*
  Cup.h - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/
#ifndef Cup_h
#define Cup_h

#include "Arduino.h"

class Cup
{
  public:
    Cup(int dPin);
    Cup(int dPin, int bPin);
    void updateState();
    bool isAvailable();
    
  private:
    bool readDetectorPin();
    int detectorPin;
    int beeperPin = -1;
    bool cupAvailable = false;
};

#endif

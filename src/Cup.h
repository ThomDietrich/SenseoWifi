/*
  Cup.h - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/
#ifndef Cup_h
#define Cup_h

#include "Homie.h"

class Cup
{
  public:
    Cup(int pin);
    void initDebouncer();
    void updateState();
    void fillUp();
    bool isAvailableChanged();
    bool isFullChanged();
    bool isAvailable();
    bool isNotAvailable();
    bool isFull();
    bool isEmpty();
  private:
    int detectorPin;
    Bounce debouncer;
    bool availableChanged = false;
    bool fullChanged = false;
    bool cupAvailable = false;
    bool cupFull = false;
};

#endif

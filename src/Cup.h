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
    bool hasChanged();
    bool isAvailable();
    bool isNotAvailable();
    bool isFull();
    bool isEmpty();
  private:
    int detectorPin;
    Bounce debouncer;
    bool changed;
    bool cupAvailable;
    bool cupFull = false;
};

#endif

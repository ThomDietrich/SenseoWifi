/*
  SenseoSM.h - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/
#ifndef SenseoSM_h
#define SenseoSM_h

#include "Arduino.h"
#include "config.h"
#include "enums.h"

class SenseoSM
{
  public:
    SenseoSM();
    void updateState(ledStateEnum ledState);
    bool stateHasChanged();
    senseoStateEnum getState();
    String getStateAsString();
    int getTimeInLastState();
  private:
    senseoStateEnum senseoState = SENSEO_unknown;
    senseoStateEnum senseoStatePrev = SENSEO_unknown;
    unsigned long lastStateChangeMillis;
    bool hasChanged;
    int timeInLastState;
    
};

#endif

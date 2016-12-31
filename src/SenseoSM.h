/*
  SenseoSM.h - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/
#ifndef SenseoSM_h
#define SenseoSM_h

#include "Arduino.h"
#include "enums.h"
#include "constants.h"

class SenseoSM
{
  public:
    SenseoSM();
    void updateState(ledStateEnum ledState);
    bool stateHasChanged();
    senseoStateEnum getState();
    senseoStateEnum getStatePrev();
    String getStateAsString();
    int getSecondsInLastState();
  private:
    senseoStateEnum senseoState = SENSEO_unknown;
    senseoStateEnum senseoStatePrev = SENSEO_unknown;
    unsigned long lastStateChangeMillis;
    bool hasChanged;
    int timeInLastState;
    
};

#endif

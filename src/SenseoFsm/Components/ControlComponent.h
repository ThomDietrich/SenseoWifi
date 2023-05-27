/*
  SenseoControl.h - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/
#pragma once

#include "ModularFsm/FsmComponent.h"

/**
 * SenseoControl includes all functions hardware control specific.
 * At the time of this writing, this is mainly "pressing" the buttons.
 */
class ControlComponent : public FsmComponent<ControlComponent>
{
public:
    ControlComponent(int pBPin, int lBPin, int rBPin);
    void pressPowerButton();
    void pressLeftButton();
    void pressRightButton();
    void pressLeftRightButton();
    void update();

private:
    /** The time one/the last button was pressed. '0' means no button is currently pressed */
    unsigned long timestampPressed = 0;
    int powerButtonPin;
    int leftButtonPin;
    int rightButtonPin;
};

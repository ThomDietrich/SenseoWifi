/*
CupComponent.cpp - Library for the SenseoWifi project.
Created by Thomas Dietrich, 2016-03-05.
Released under some license.
*/

#include "CupComponent.h"

#include "Homie.h"
#include "constants.h"

CupComponent::CupComponent(HomieNode &node, int pin)
    : senseoNode(node), detectorPin(pin)
{
    pinMode(detectorPin, INPUT_PULLUP);
    cupAvailable = !digitalRead(detectorPin);

    senseoNode.setProperty("cupAvailable").send(isAvailable() ? "true" : "false");
    senseoNode.setProperty("cupFull").send(isFull() ? "true" : "false");
}

void CupComponent::update()
{
    bool value = !digitalRead(detectorPin);
    // debounce
    if (value != lastChangeValue)
    {
        lastChangeValue = value;
        lastChangeMillis = millis();
    }
    if (millis() - lastChangeMillis <= CupDebounceInterval)
        return;
    // process debounced detector reading
    if (value != cupAvailable)
    {
        cupAvailable = value;
        cupFilling = false;
        senseoNode.setProperty("cupAvailable").send(isAvailable() ? "true" : "false");
        if (cupAvailable || (!cupAvailable && cupFull))
        {
            cupFull = false;
            senseoNode.setProperty("cupFull").send(isFull() ? "true" : "false");
        }
    }
}

void CupComponent::setFilling()
{
    cupFilling = true;
    cupFull = false;
}

void CupComponent::setFull()
{
    cupFilling = false;
    cupFull = true;
    senseoNode.setProperty("cupFull").send(isFull() ? "true" : "false");
}

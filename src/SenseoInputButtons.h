/*
  SenseoInputButtons.h - Library for the SenseoWifi project.
  Released under some license.
*/
#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <set>

/*#include "Homie.h"
#include "enums.h"
#include "constants.h"*/

/**
 * SenseoInputButtons is use to associate callback with Senseo Buttons
 */
class SenseoInputButtons
{
public:
    typedef std::function<void()> ButtonHandler;

    SenseoInputButtons(int analogPin);

    void addButtonPushHandler(int buttonValue, const ButtonHandler &handler);
    void addButtonHoldHandler(int buttonValue, unsigned long time, const ButtonHandler &handler);
    void addButtonReleaseHandler(int buttonValue, unsigned long time, const ButtonHandler &handler);

    void update();

private:
    enum HandlerType
    {
        ON_PUSH,
        ON_HOLD,
        ON_RELEASE,
    };

    struct HandlerData
    {
        HandlerType type;
        int buttonValue;
        ButtonHandler handler;
        unsigned long time = 0; // use for Hold and Release Handler
    };

    struct DelayedHandler
    {
        ButtonHandler handler;
        unsigned long timeToDiscard;
    };

    bool hasHandler(int reading) const;
    bool isReleased(int reading) const;
    ButtonHandler findBestReleaseHandler(int reading) const;
    void AddToDelayedReleaseHandler(ButtonHandler handler, unsigned long timeout);
    unsigned long lastReadingTime = 0;
    unsigned long lastReadingChangeTime = 0;
    int analogPin;
    int previousReading = 0;

    std::set<int> handlersByValue;
    std::vector<HandlerData> handlers;
    std::vector<std::unique_ptr<DelayedHandler>> delayedReleaseHandlers;
};

/*
  config.h - Configuration for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#ifndef SenseoConstants_h
#define SenseoConstants_h

// LED state machine
// Measure your timings by activating debugging in ledChangedRoutine()
static const int pulseDurLedSlow = 1000;                   // duration of one pulse when LED is blinking slow in milliseconds (state LED_SLOW)
static const int pulseDurLedFast = 100;                    // duration of one pulse when LED is blinking fast in milliseconds (state LED_FAST)
static const int pulseDurTolerance = 10;                   // tolerance for pulse duration. With the tested senseo, tolerance was not more than +-1ms
static const int pulseContThreshold = 2 * pulseDurLedSlow; // time before switching to continuous LED state (state LED_ON and state LED_OFF)

// Senseo state machine (in seconds)
static const int HeatingTime = 70;
static const int HeatingTimeTol = 15;
static const int Brew1CupSeconds = 21;
static const int Brew2CupSeconds = 41;
static const int Brew1CupMillies = Brew1CupSeconds * 1000;
static const int Brew2CupMillies = Brew2CupSeconds * 1000;
static const int BrewHeat1CupSeconds = 55;
static const int BrewHeat2CupSeconds = 85;

// Debouncing (in milliseconds)
static const int CupDebounceInterval = 500;
static const int LedIgnoreChangeDuration = 5;

// Duration of button "press" (in milliseconds)
static const int pressDuration = 150;

//Button threesholds for A0
static const int A0buttonPwr = 952;
static const int A0button1Cup = 733;
static const int A0button2Cup = 895;
static const int A0buttonPwr1Cup = 667;
static const int A0buttonPwr1Cup2Cup = 592;
static const int A0buttonPwr2Cup = 804;
static const int A0button1Cup2Cup = 640;
static const int A0buttonThreeshold = 25;
static const int A0NoButtonPress = 1024;

#endif

/*
config.h - Configuration for the SenseoWifi project.
Created by Thomas Dietrich, 2016-03-05.
Released under some license.
*/

#pragma once

// circuit pin definitions      // PCB v1.6
#define ocPressLeftPin D5       // T1
#define ocPressRightPin D2      // T2
#define ocPressPowerPin D1      // TP
#define ocSenseLedPin D7        // LED
#define cupDetectorPin D6       // TCRT5000 D0
#define beeperPin D8            // BUZZER
#define resetButtonPin D4       //

// used only with the Customizable Buttons Addon
#define senseoLedOutPin D4         // this is the same as the reset pin which is fine as the reset button is no more need with the addon
#define senseoButtonsInputPin A0  // 

// Reset: D3 doesn't work (why?), D0 okay, D4 needs LED off

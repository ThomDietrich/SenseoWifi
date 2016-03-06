/*
  enums.h - part of the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#ifndef SenseoEnums_h
#define SenseoEnums_h

enum ledStateEnum {
  LED_unknown,
  LED_OFF,
  LED_SLOW,
  LED_FAST,
  LED_ON
};

enum senseoStateEnum {
  SENSEO_unknown,
  SENSEO_OFF,
  SENSEO_HEATING,
  SENSEO_READY,
  SENSEO_BREWING,
  SENSEO_NOWATER
};

#endif

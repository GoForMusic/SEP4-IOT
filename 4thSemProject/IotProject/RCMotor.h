#pragma once

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <event_groups.h>

#include "TemperatureHumiditySensor.h"
#include "rc_servo.h"
#include "Configuration.h"
#include "Co2Sensor.h"
#include "Setup.h"

int isOpen;

void createBoxController();
void openBox();
void closeBox();
void boxControllerTask(void *pvParameter);
void createBoxControllerTask(UBaseType_t uxPriority);
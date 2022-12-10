﻿#pragma once

#include <stdint.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>
#include <hih8120.h>
#include <task.h>
#include <semphr.h>
#include "Setup.h"



void create(UBaseType_t Taskpriority);
uint16_t getHumidity();
uint16_t getTemperature();
void initializeTempAndHumDriver();
void measureTempAndHum();
void TempAndHumTask(void* pvpParameter);
void createTempAndHumTask(UBaseType_t Taskpriority);
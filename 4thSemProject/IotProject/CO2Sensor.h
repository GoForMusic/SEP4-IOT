#pragma once

#include <stdint.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <task.h>
#include <semphr.h>
#include <stdbool.h>
#include <serial.h>
#include <mh_z19.h>
#include "Setup.h"


void create(UBaseType_t Taskpriority);
uint16_t getCo2();
void initializeCo2Driver();
void measureCo2();
void Co2Task(void* pvpParameter);
void createCo2Task(UBaseType_t Taskpriority);
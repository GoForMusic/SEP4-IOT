#pragma once

#include <stdint.h>
#include <stdio.h>
#include <ATMEGA_FreeRTOS.h>
#include <hih8120.h>

uitn16_t getTemperature();
uitn16_t getHumidity();
void initializeTempAndHumidityDriver();
void measureTempAndHumidity();

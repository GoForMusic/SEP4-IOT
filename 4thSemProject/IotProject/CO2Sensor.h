#pragma once
#include <mh_z19.h>
#include <ATMEGA_FreeRTOS.h>


uint16_t getCO2();
void initializeCO2();
void measureCO2();
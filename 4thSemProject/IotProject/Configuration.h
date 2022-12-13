

#pragma once

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>


extern uint16_t co2BenchMark;
extern uint16_t tempBenchMark;
extern uint16_t humBenchMark;

extern SemaphoreHandle_t configMutex;

void createConfiguration();

uint16_t getCo2BenchMark();
uint16_t getTempBenchMark();
uint16_t getHumBenchMark();

void setCo2BenchMark(uint16_t benchMark);
void setTempBenchMark(uint16_t benchMark);
void setHumBenchMark(uint16_t benchMark);
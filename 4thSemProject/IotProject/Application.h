#pragma once

#include "TemperatureAndHumiditySensor.h"
#include "SensorDataPackageHandler.h"
#include "Co2Sensor.h"
#include "Setup.h"
#include "message_buffer.h"
#include <ATMEGA_FreeRTOS.h>
#include "lora_driver.h"
#include "Configuration.h"
#include <event_groups.h>

void createApplicationTask(UBaseType_t Taskpriority);
void ApplicationTask(void *pvParameters);

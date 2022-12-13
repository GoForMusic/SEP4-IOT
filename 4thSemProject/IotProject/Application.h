#pragma once

#include <ATMEGA_FreeRTOS.h>
#include <event_groups.h>
#include "TemperatureHumiditySensor.h"
#include "SensorDataPackageHandler.h"
#include "Setup.h"
#include "message_buffer.h"
#include "lora_driver.h"
#include "Co2Sensor.h"
#include "Configuration.h"

void createApplicationTask(UBaseType_t Taskpriority);
void ApplicationTask(void *pvParameters);
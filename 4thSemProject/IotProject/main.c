﻿#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <serial.h>
#include <event_groups.h>

 // Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>

#include "Application.h"
#include "TemperatureHumiditySensor.h"
#include "Co2Sensor.h"
#include "Setup.h"
#include "UpLinkHandler.h"
#include "DownLinkHandler.h"
#include "Configuration.h"
#include "RCMotor.h"

void initialise(void) // Initialise 
{
 initializeEventGroup();
 createUpLinkMessageBuffer();
 createDownLinkMessageBuffer();
 createConfiguration();
 createBoxController();
 lora_driver_initialise(ser_USART1, downLinkMessageBuffer);
}

void create_tasks(void) // Create tasks
{
 createTempAndHumTask(1);
 createCo2Task(1);
 createApplicationTask(2);
 lora_handler_uplink_payload(4);
 lora_handler_downlink_create(3);
 createRCMotorTask(2);
 
}

void initSystem(void)
{
 
 DDRA |= _BV(DDA0) | _BV(DDA7); // Set PA0 and PA7 as output

 // Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
 initializeUsedData();
 stdio_initialise(ser_USART0);
 create_tasks();


 status_leds_initialise(5); // Priority 5 for internal task; LoraWan intialisation is done in the task
}

int main(void)
{
 initSystem(); // Initialise system
 printf("\nStarted\n");
 vTaskStartScheduler(); // Start FreeRTOS scheduler. Execution will never return from this function.
 while (1)
 {
 }
}
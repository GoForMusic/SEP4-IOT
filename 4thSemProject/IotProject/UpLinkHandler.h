/*
#pragma once

#include <stdint.h>



void UplinkHandler_createTask(void);
void UplinkHandlerTask(void);
static void _lora_setup(void);
static void reset_lora_module(void);
*/

#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <status_leds.h>
#include "SensorDataPackageHandler.h"
#include "message_buffer.h"
#include "Setup.h"

// Parameters for OTAA join - You have got these in a mail from IHA
#define LORA_appEUI "49B360EEE16A8D4C"
#define LORA_appKEY "E0597BF885F1F18CF896B91F8E211814"

void lora_handler_uplink_payload(UBaseType_t TaskPriority);
void _lora_setup(void);
void lora_handler_task( void *pvParameters );

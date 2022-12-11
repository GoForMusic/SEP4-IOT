#pragma once

#include <stdint.h>

/**
 * Create the UplinkHandler task
 */

void UplinkHandler_createTask(void);
void UplinkHandlerTask(void);
static void _lora_setup(void);
static void reset_lora_module(void);


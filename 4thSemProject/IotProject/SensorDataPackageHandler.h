/*
 * SensorDataPackageHandler.h
 *
 * Created: 07/12/2022 18:22:49
 *  Author: Sami
 */ 

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>


void setCO2(uint16_t _co2);

void setHumidity(uint16_t _humidity);

void setTemperature(uint16_t _temperature);

lora_driver_payload_t getLoRaPayload(uint16_t port_no);
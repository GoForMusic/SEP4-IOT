/*
 * SensorDataPackageHandler.h
 *
 * Created: 07/12/2022 18:22:49
 *  Author: Sami
 */ 

#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <status_leds.h>

void setCos2Ppm(uint16_t _co2Ppm);

void setHumidityX10Percent(uint16_t _humidityX10Percent);

void setTemperatureX10C(uint16_t _temperatureX10C);

lora_driver_payload_t getLoRaPayload(uint16_t port_no);
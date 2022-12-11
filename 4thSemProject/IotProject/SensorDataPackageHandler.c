/*
 * SensorDataPackageHandler.c
 *
 * Created: 07/12/2022 18:22:49
 *  Author: Sami
 */ 

#include "SensorDataPackageHandler.h"



uint16_t co2Ppm;
uint16_t humidityX10Percent;
uint16_t temperatureX10C;


void setCos2Ppm(uint16_t _co2Ppm)
{
	co2Ppm=_co2Ppm;

}

void setHumidityX10Percent(uint16_t _humidityX10Percent)
{
	humidityX10Percent=_humidityX10Percent;

}

void setTemperatureX10C(uint16_t _temperatureX10C)
{
	temperatureX10C=_temperatureX10C;

}

lora_driver_payload_t getLoRaPayload(uint16_t port_no)
{
	lora_driver_payload_t payload;
	payload.len = 6;
	payload.portNo = port_no;
	payload.bytes[0] = humidityX10Percent >> 8;
	payload.bytes[1] = humidityX10Percent & 0xFF;
	payload.bytes[2] = temperatureX10C >> 8;
	payload.bytes[3] = temperatureX10C & 0xFF;
	payload.bytes[4] = co2Ppm >> 8;
	payload.bytes[5] = co2Ppm & 0xFF;
	return payload;
}
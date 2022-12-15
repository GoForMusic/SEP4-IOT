/*
 * SensorDataPackageHandler.c
 *
 * Created: 07/12/2022 18:22:49
 *  Author: Sami
 */ 

#include "SensorDataPackageHandler.h"



uint16_t co2;
uint16_t humidity;
uint16_t temperature;


void setCO2(uint16_t _co2) // setter for co2
{
	co2=_co2;
}

void setHumidity(uint16_t _humidity) // setter for humidity
{
	humidity=_humidity;
}

void setTemperature(uint16_t _temperature) // setter for temperature
{
	temperature=_temperature;
}

lora_driver_payload_t getLoRaPayload(uint16_t port_no)
{
	lora_driver_payload_t* payload; // create a payload
	payload = pvPortMalloc(sizeof(lora_driver_payload_t)); // allocate memory for the payload
	if(payload == NULL) // if the allocation failed
    {
        printf("\nFailed to allocate memory for payload\n");
    }
	else 
	{
		
		payload->len=6;	// set the length of the payload
		payload->bytes[0]=co2 >> 8; // set the first byte of the payload to the first byte of the co2 value
		payload->bytes[1]=co2 & 0xFF; // set the second byte of the payload to the second byte of the co2 value
		payload->bytes[2]=humidity >> 8; // set the third byte of the payload to the first byte of the humidity value
		payload->bytes[3]=humidity & 0xFF; // set the fourth byte of the payload to the second byte of the humidity value
		payload->bytes[4]=temperature >> 8; // set the fifth byte of the payload to the first byte of the temperature value
		payload->bytes[5]=temperature & 0xFF;// set the sixth byte of the payload to the second byte of the temperature value
			
	}
	return *payload; // return the payload
}


#include "TemperatureAndHumiditySensor.h"

uint16_t temp = 0;
uint16_t hum = 0;


uint16_t getHumidity()
{
	return hum;
}


uint16_t getTemperature()
{
	return temp;
}


void create(UBaseType_t Taskpriority) // Create task
{
	initializeTempAndHumDriver();
	createTempAndHumTask(Taskpriority);
	
}


void initializeTempAndHumDriver() // initialise the sensor
{
	

	if ( HIH8120_OK == hih8120_initialise() )
	{
		printf("Temp and Hum Driver Initialized ok\n");
	}
	
	else {
		printf("Temp and hum out of heap \n");
	}
}


void measureTempAndHum() // wake up 
{
	if ( HIH8120_OK != hih8120_wakeup() )
	{
		printf("Something went wrong\n");
	}
	
	vTaskDelay(pdMS_TO_TICKS(50)); // wait for sensor to wake up
	
	if ( HIH8120_OK !=  hih8120_measure() )
	{
		printf("Something went wrong\n");
	}
	
	vTaskDelay(pdMS_TO_TICKS(20)); // wait for measurement to complete
}


void TempAndHumTask(void* pvpParameter)
{
	
	while(1)
	{
		EventBits_t eventBits = xEventGroupWaitBits(measureEventGroup,HUMIDITY_TEMPERATURE_MEASURE_BIT,pdTRUE,pdTRUE,portMAX_DELAY);
		if(eventBits & (HUMIDITY_TEMPERATURE_MEASURE_BIT))
		{
			puts("Measuring...");
			measureTempAndHum();
			temp = hih8120_getTemperature();
			hum = hih8120_getHumidity();
			xEventGroupSetBits(dataReadyEventGroup,HUMIDITY_TEMPERATURE_READY_BIT);
		}
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}


void createTempAndHumTask(UBaseType_t Taskpriority)
{
	initializeTempAndHumDriver(); // initialize the sensor
	xTaskCreate( 
	TempAndHumTask // Task function
	,  "TempAndHumTask" // Task name
	,  configMINIMAL_STACK_SIZE // Stack size
	,  NULL // Parameter
	,  tskIDLE_PRIORITY + Taskpriority // Priority
	,  NULL ); // Task handle
}
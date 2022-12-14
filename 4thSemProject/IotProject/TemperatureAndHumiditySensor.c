

#include "TemperatureHumiditySensor.h"

//Initializing temperature and humidity as 0. 
uint16_t temperature = 0;
uint16_t humidity = 0;

//Getter for humidity.
uint16_t getHumidity()
{
	return humidity;
}

//Getter for tempearature.
uint16_t getTemperature()
{
	return temperature;
}


void create(UBaseType_t Taskpriority) // Create task
{
	initializeTempAndHumDriver();
	createTempAndHumTask(Taskpriority);
	
}


void initializeTempAndHumDriver() // initialise the sensor
{
	hih8120_driverReturnCode_t returnCode = hih8120_initialise();

	if ( HIH8120_OK == returnCode )
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

/*
 A function that waits until the application requests measures through an event group. 
When the measure function is called and measurements are required, temperature and humidity are measured and set to those values.
*/
void TempAndHumTask(void* pvpParameter)
{
	
	while(1)
	{
		EventBits_t eventBits = xEventGroupWaitBits(measureEventGroup,HUMIDITY_TEMPERATURE_MEASURE_BIT,pdTRUE,pdTRUE,portMAX_DELAY);
		if(eventBits & (HUMIDITY_TEMPERATURE_MEASURE_BIT))
		{
			puts("Measuring metrics...");
			measureTempAndHum();
			temperature = hih8120_getTemperature();
			humidity = hih8120_getHumidityPercent();
			xEventGroupSetBits(dataReadyEventGroup,HUMIDITY_TEMPERATURE_READY_BIT);
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

// A function that creates temperature and humidity task.
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
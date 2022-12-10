//A class that handles temperature and humidity measuring.

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

/*
A function that initializes the temp and hum driver and creates temperature and humidity task.
*/
void create(UBaseType_t Taskpriority)
{
	initializeTempAndHumDriver();
	createTempAndHumTask(Taskpriority);
	
}

/*
A function that initializes temperature and humidity driver and depending on the response of the driver
returns a code.
*/
void initializeTempAndHumDriver()
{
	hih8120_driverReturnCode_t returnCode = hih8120_initialise();

	if ( HIH8120_OK == returnCode )
	{
		printf("Temp and Hum Driver Initialized ok\n");
	}
	
	else {
		printf("TEMP AND HUM OUT OF HEAP \n");
	}
}

/*
A function that wakes up the driver and measures temperature and humidity.
*/
void measureTempAndHum()
{
	if ( HIH8120_OK != hih8120_wakeup() )
	{
		printf("TEMP AND HUM WAKE UP WENT WRONG\n");
	}
	
	vTaskDelay(pdMS_TO_TICKS(50));
	
	if ( HIH8120_OK !=  hih8120_measure() )
	{
		printf("TEMP AND HUM MEASURING UP WENT WRONG\n");
	}
	
	vTaskDelay(pdMS_TO_TICKS(20));
}

/*
 A function, that waits until Application asks for measurements through event group,
 When the measurements are needed, the measure function is called, temperature and humidity,
 are measured and temperature and humidty are set to those measurements. 
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
			temperature = hih8120_getTemperature_x10();
			humidity = hih8120_getHumidityPercent_x10();
			xEventGroupSetBits(dataReadyEventGroup,HUMIDITY_TEMPERATURE_READY_BIT);
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

// A function that creates temperature and humidity task.
void createTempAndHumTask(UBaseType_t Taskpriority)
{
	initializeTempAndHumDriver();
	xTaskCreate(
	TempAndHumTask
	,  "TempAndHumTask"
	,  configMINIMAL_STACK_SIZE
	,  NULL
	,  tskIDLE_PRIORITY + Taskpriority
	,  NULL );
}
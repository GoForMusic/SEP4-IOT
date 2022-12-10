#include "Co2Sensor.h"

void myCo2CallBack(uint16_t ppm);

uint16_t co2Ppm = 0;
mh_z19_returnCode_t rc;

//Initializing the CO2 driver to handle measurements
void initializeCo2Driver(){
	mh_z19_initialise(ser_USART3);
	mh_z19_injectCallBack(myCo2CallBack);
}

//Task for handling measurements when Application asks
void Co2Task(void* pvpParameter){
	while(1){
		
		//EventGroup waits until Application sends MEASURE_BIT and tells each of the sensors to start measuring the metrics
		EventBits_t eventBits = xEventGroupWaitBits(measureEventGroup,CO2_MEASURE_BIT,pdTRUE,pdTRUE,portMAX_DELAY);
		
		//Verifies if the MEASURE_BIT has CO2_MEASURE_BIT included
		if(eventBits & (CO2_MEASURE_BIT)){
			printf("Measuring CO2... \n");
			
			//If yes, then the driver starts measuring the CO2
			measureCo2();
			
			/*Event Group sets the CO2_READY_BIT - When all 'SENSOR'_READY_BIT's are set, 
			then the application sets the metrics and sends the payload_package.*/
			//***See Application class***
			xEventGroupSetBits(dataReadyEventGroup,CO2_READY_BIT);
			
			//Task is delayed with 10 milliseconds.
			vTaskDelay(pdMS_TO_TICKS(10));
		}
	}
}

//Callback to set the CO2 data
void myCo2CallBack(uint16_t ppm)
{
	co2Ppm = ppm;
}


//Gets CO2 data
uint16_t getCo2(){
	return co2Ppm;
}


//Measures the CO2 data
void measureCo2(){
	
	//takes a measurement
	rc = mh_z19_takeMeassuring();
	
	//if the measurement went wrong then it prints out an Error Message
	if (rc != MHZ19_OK)
	{
		printf("Something went wrong with CO2 sensor");
	}
	
	//if not, then it calls "myCo2CallBack" method which is injected 
}

//creates the Co2Task which is used in main.c class
void createCo2Task(UBaseType_t Taskpriority){
	initializeCo2Driver(); // initializes the CO2 driver
	xTaskCreate(
	Co2Task,  //method
	"Co2Task",	//Name of method
	configMINIMAL_STACK_SIZE,  //The size of the stack to configurate the method
	NULL, // (void *pvParameters)
	tskIDLE_PRIORITY + Taskpriority,  //the priority of the task
	NULL //No TaskHandle created.
	);
}
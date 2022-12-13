#include "Co2Sensor.h"

void myCo2CallBack(uint16_t ppm);

uint16_t co2Ppm = 0;
mh_z19_returnCode_t rc;

//Initializing the CO2 driver to handle measurements
void initializeCo2Driver(){
	mh_z19_initialise(ser_USART3);
	mh_z19_injectCallBack(myCo2CallBack);
}


void Co2Task(void* pvpParameter) 
{
	while(1){
		
		
		EventBits_t eventBits = xEventGroupWaitBits(measureEventGroup,CO2_MEASURE_BIT,pdTRUE,pdTRUE,portMAX_DELAY); // wait for Application to set MEASURE_BIT and tells each of the sensors to start measuring
		
		
		if(eventBits & (CO2_MEASURE_BIT)) //if MEASURE_BIT has CO2_MEASURE_BIT
			{
			printf("Measuring CO2... \n");
			
			
			measureCo2(); //measure CO2
			}
			
			
			xEventGroupSetBits(dataReadyEventGroup,CO2_READY_BIT); //set CO2_READY_BIT to dataReadyEventGroup
		
			vTaskDelay(pdMS_TO_TICKS(10)); //delay for 10ms
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
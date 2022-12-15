#include "Application.h"


void ApplicationTask(void *pvParameters) 
{
	for (;;)
	{
		
		xEventGroupSetBits(measureEventGroup, ALL_MEASURE_BITS); // set all bits in the event group for all tasks to start measuring
		
		
		EventBits_t eventBits = xEventGroupWaitBits(dataReadyEventGroup,ALL_READY_BITS,pdTRUE,pdTRUE,portMAX_DELAY); // wait for all tasks to finish measuring and set their bits in the event group
		
		if(eventBits & (ALL_READY_BITS))
		{
			printf("\nAll the data has been collected\n");
			printf("Temperature is: ,\n and humidity is: , \n CO2 is:  \n",getTemperature(),getHumidity(),getCo2());
			
			
			setTemperature(getTemperature());
			setHumidity(getHumidity());
			setCO2(getCo2());
			
			
			
			
			lora_driver_payload_t _uplink_payload = getLoRaPayload(2);
			xMessageBufferSend(upLinkMessageBuffer,&_uplink_payload,sizeof(_uplink_payload),portMAX_DELAY);
											
			//Application is delayed with 10 minutes until the next measurement starts.										
			vTaskDelay(pdMS_TO_TICKS(600000));
		}
		
		//Application with other 50 milliseconds until the next measurement starts.
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}



void createApplicationTask(UBaseType_t Taskpriority)
{
	xTaskCreate(
	ApplicationTask	//method
	,  "AppTask" //Name of method
	,  configMINIMAL_STACK_SIZE	//The size of the stack to configure the method
	,  NULL // (void *pvParameters)
	,  tskIDLE_PRIORITY + Taskpriority //the priority of the task
	,  NULL ); //No TaskHandle created.
}
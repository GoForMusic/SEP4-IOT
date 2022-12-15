/*
#include <lora_driver.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include "UpLinkHandler.h"

//for OTAA
#define LORA_APP_EUI "F0F0F0F0F0F0F0F0"; //the application EUI
#define LORA_APP_KEY "F0F0F0F0F0F0F0F0"; //the application key

uint16_t humidity; //the humidity value
int16_t temperature; //the temperature value


static void reset_lora_module(void)
{
        //reseting the hardware of the LoRaWAN module
        lora_driver_resetRn2483(1); //resetting the module
        vTaskDelay(2000); //wait for the module to reset
        lora_driver_resetRn2483(0); //stop resetting the module
        vTaskDelay(150); //wait for the module to reset
        lora_driver_flushBuffers(); //get rid of any garbage in the buffers
}

static void _lora_setup(void)
{
    //OTAA setup
    //s1
    if  (lora_driver_rn2483FactoryReset() != LORA_OK) //reset the module
    {
        printf("Failed to reset the module\r\n");
        return;
    }

    //s2
    if (lora_driver_configureToEu868() != LORA_OK) //configure the module to EU868
    {
        printf("Failed to configure the module to EU868\r\n");
        return;
    }

    //s3
    static char devEui[17]; //it is static because it is needed to keep the value after the function returns
    if (lora_driver_getRn2483AppEui(appEui) != LORA_OK) //get the application EUI
    {
        printf("Failed to get the application EUI\r\n");
        return;
    }

    //s4
    if (lora_driver_setOtaaIdentity(LORA_APP_EUI, LORA_APP_KEY, devEui) != LORA_OK) //set the application EUI, application key and device EUI
    {
        printf("Failed to set the application EUI, application key and device EUI\r\n");
        return;
    }

    //s5
    if (lora_driver_saveMac() != LORA_OK) //save the MAC
    {
        printf("Failed to save the MAC\r\n");
        return;
    } //the module is now configured

    //s6
    if (lora_driver_join(LORA_OTAA) == LORA_ACCEPTED) //join the network
    {
        printf("Joined the network\r\n");
        return;
    }

    //uplink message setup
    //s1 define a payload variable
    lora_driver_payload_t uplinkPayload; //the payload of the uplink message

    //s2 populate the payload with the data
    uplinkPayload.len = 6; //the length of the payload; 2 bytes for the temperature and 2 bytes for the humidity and 2 bytes for the CO2 value
    uplinkPayload.port = 1; //the port of the payload

    uplinkPayload.bytes[0] = humidity >> 8; //the first byte of the payload is the most significant byte of the humidity value
    uplinkPayload.bytes[1] = humidity & 0xFF; //the second byte of the payload is the least significant byte of the humidity value
    uplinkPayload.bytes[2] = temperature >> 8; //the third byte of the payload is the most significant byte of the temperature value
    uplinkPayload.bytes[3] = temperature & 0xFF; //the fourth byte of the payload is the least significant byte of the temperature value
    uplinkPayload.bytes[4] = co2 >> 8; //the fifth byte of the payload is the most significant byte of the CO2 value
    uplinkPayload.bytes[5] = co2 & 0xFF; //the sixth byte of the payload is the least significant byte of the CO2 value

    //s3 send the uplink message
    lora_Driver_returnCode_t rc; //the return code of the send function

    if ((rc = lora_driver_sendUploadMessage(false, &_uplinkPayload)) == LORA_MAC_TX_OK) //send the uplink message
    {
        printf("Failed to send the uplink message\r\n");
        return;
    }
    else if (rc == LORA_MAC_TX_OK)
    {
        printf("Uplink message sent\r\n");
        return;
    }
}

void UplinkHandlerTask(void *pvParameters)
{
    reset_lora_module(); // Reset LoRa module
    lorawan_driver_initialize(ser_USART1, NULL); // Initialize LoRaWAN driver
    lorawan_driver_join(OTAA); // Join LoRaWAN network
    for (;;)
    {
        lorawan_driver_send(1, "message body", 11, 1, 0); // Send message
        vTaskDelay(pdMS_TO_TICKS(10000)); // Wait 10 seconds
    }
    vTaskDelete(NULL); // Delete task
}

void UplinkHandler_createTask() // Create task for UplinkHandler
{
    //create task
    xTaskCreate(UplinkHandler, "UplinkHandler", 100, NULL, 1, NULL);
}
*/
#include "UpLinkHandler.h"

lora_driver_payload_t uplink_payload;

void lora_handler_task( void *pvParameters );

void lora_handler_uplink_payload(UBaseType_t TaskPriority)
{
	xTaskCreate(
	lora_handler_task
	,  "Uplink_payload_handler_task"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+200  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  TaskPriority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

 void _lora_setup(void)
{
	char _out_buf[20];
	lora_driver_returnCode_t rc;
	status_leds_slowBlink(led_ST2); // OPTIONAL: Led the green led blink slowly while we are setting up LoRa

	// Factory reset the transceiver
	printf("FactoryReset >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_rn2483FactoryReset()));
	
	// Configure to EU868 LoRaWAN standards
	printf("Configure to EU868 >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_configureToEu868()));

	// Get the transceivers HW EUI
	rc = lora_driver_getRn2483Hweui(_out_buf);
	printf("Get HWEUI >%s<: %s\n",lora_driver_mapReturnCodeToText(rc), _out_buf);

	// Set the HWEUI as DevEUI in the LoRaWAN software stack in the transceiver
	printf("Set DevEUI: %s >%s<\n", _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setDeviceIdentifier(_out_buf)));

	// Set Over The Air Activation parameters to be ready to join the LoRaWAN
	printf("Set OTAA Identity appEUI:%s appKEY:%s devEUI:%s >%s<\n", LORA_appEUI, LORA_appKEY, _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setOtaaIdentity(LORA_appEUI,LORA_appKEY,_out_buf)));

	// Save all the MAC settings in the transceiver
	printf("Save mac >%s<\n",lora_driver_mapReturnCodeToText(lora_driver_saveMac()));

	// Enable Adaptive Data Rate
	printf("Set Adaptive Data Rate: ON >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_setAdaptiveDataRate(LORA_ON)));

	// Set receiver window1 delay to 500 ms - this is needed if down-link messages will be used
	printf("Set Receiver Delay: %d ms >%s<\n", 500, lora_driver_mapReturnCodeToText(lora_driver_setReceiveDelay(500)));

	// Join the LoRaWAN
	uint8_t maxJoinTriesLeft = 10;
	
	do {
		rc = lora_driver_join(LORA_OTAA);
		printf("Join Network TriesLeft:%d >%s<\n", maxJoinTriesLeft, lora_driver_mapReturnCodeToText(rc));

		if ( rc != LORA_ACCEPTED)
		{
			// Make the red led pulse to tell something went wrong
			status_leds_longPuls(led_ST1); // OPTIONAL
			// Wait 5 sec and lets try again
			vTaskDelay(pdMS_TO_TICKS(5000UL));
		}
		else
		{
			break;
		}
	} while (--maxJoinTriesLeft);

	if (rc == LORA_ACCEPTED)
	{
		// Connected to LoRaWAN :-)
		// Make the green led steady
		status_leds_ledOn(led_ST2); // OPTIONAL
	}
	else
	{
		// Something went wrong
		// Turn off the green led
		status_leds_ledOff(led_ST2); // OPTIONAL
		// Make the red led blink fast to tell something went wrong
		status_leds_fastBlink(led_ST1); // OPTIONAL

		// Lets stay here
		while (1)
		{
			taskYIELD();
		}
	}
}

/*-----------------------------------------------------------*/
void lora_handler_task( void *pvParameters )
{
	// Hardware reset of LoRaWAN transceiver
	lora_driver_resetRn2483(1);
	vTaskDelay(2);
	lora_driver_resetRn2483(0);
	// Give it a chance to wakeup
	vTaskDelay(150);

	lora_driver_flushBuffers(); // get rid of first version string from module after reset!

	_lora_setup();

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(120000UL); // Upload message every 10 minutes (600000 ms)
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );

		
		xMessageBufferReceive(upLinkMessageBuffer,&uplink_payload,sizeof(uplink_payload),portMAX_DELAY);
		status_leds_shortPuls(led_ST4);  
		printf("Upload Message >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &uplink_payload)));
	}
} 

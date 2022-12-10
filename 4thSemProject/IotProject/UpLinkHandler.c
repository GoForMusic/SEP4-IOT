#include <lora_driver.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>

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

void UplinkHandler_createTask(); // Create task for UplinkHandler
{
    //create task
    xTaskCreate(UplinkHandler, "UplinkHandler", 100, NULL, 1, NULL);
}
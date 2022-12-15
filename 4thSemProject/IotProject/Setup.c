#include "Setup.h"
EventGroupHandle_t measureEventGroup;
EventGroupHandle_t dataReadyEventGroup;
MessageBufferHandle_t upLinkMessageBuffer;
MessageBufferHandle_t downLinkMessageBuffer;

void initializeEventGroup() 
{
    dataReadyEventGroup = xEventGroupCreate();
    measureEventGroup = xEventGroupCreate();
    upLinkMessageBuffer = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2);
    downLinkMessageBuffer = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2);
}


void createUpLinkMessageBuffer() //Created a buffer for messages so it can be uploaded and send in websockets
{	
    if(upLinkMessageBuffer == NULL )
    {
        printf("Not enoughp memory to create uplink message buffer");
    }
    else
    {
        printf("Uplink message buffer created successfully");
    }
}

void createDownLinkMessageBuffer() 
{
    if(downLinkMessageBuffer == NULL )
    {
        printf("Not enough heap memory for downlink message buffer");
    }
    else
    {
        printf("Downlink message buffer created successfully\n");
    }
}
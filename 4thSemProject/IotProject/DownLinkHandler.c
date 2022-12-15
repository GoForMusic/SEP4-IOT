

#include <stddef.h>
#include <stdio.h>
#include <ATMEGA_FreeRTOS.h>
#include <lora_driver.h>
#include "Configuration.h"
#include <rc_servo.h> // Include the servo driver

//static TickType_t xLastWakeTime; // The time at which the task was last unblocked.
static const TickType_t xFrequency = pdms_TO_TICKS(DOWNLINK_HANDLER_TASK_DELAY_MS); //pdms_TO_TICKS is a macro that converts milliseconds to ticks 

#define SERVO_PIN 0; // The pin the servo is connected to
#define SERVO_MIN 0; // The minimum value the servo can take
#define ROTATE_ANGLE_OPEN 90; // The angle to rotate the servo to

void rotateServoOpenBox(int8_t servoPin, int8_t percent)
{
    //make the servo rotate 90 degrees
    rc_servo_setPosition(servoPin, percent);
}

void rotateServoCLoseBox(int8_t servoPin, int8_t percent)
{
    //make the servo rotate -90 degrees i.e. close the box
    rc_servo_setPosition(servoPin, percent);
}


void downLinkHandler_task_run()
{
    xTaskDelayUntil(&xLastWakeTime, xFrequency); // Wait for the next cycle.

    size_t bytesReceived = xMessageBufferReceive(downLinkMessageBuffer, downLinkMessage, DOWNLINK_MESSAGE_BUFFER_SIZE, 0); // Receive the message from the message buffer.

    printf("Received message from downlink");
}



void downLinkHandler_task(void *pvParameters)
{
    xLastWakeTime = xTaskGetTickCount();
    lora_driver_payload_t payload;

    void rc_servo_initialise(void); // Initialise the servo driver
    rotateServoOpenBox(SERVO_PIN, ROTATE_ANGLE_OPEN 90); // Rotate the servo to open the box
    
    for(;;)
    {
        vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_PERIOD_MS);
        downLinkHandler_task_run();
        xMessageBufferReceive(downLinkMessageHandle, &payload, sizeof(payload), portMAX_DELAY);
    }

    rotateServoCLoseBox(SERVO_PIN, SERVO_MIN); // Rotate the servo to close the box
}



void downLinkHandler_task_create(void)
{
    xTaskCreate(downLinkHandler_task, "downLinkHandler_task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}


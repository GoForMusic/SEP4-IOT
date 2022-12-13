#include "RCMotor.h"

void create createBoxController() // create a new controller
{
  rc_servo_initialise(); // initialise the controller
  isOpen = 0; // set the initial state of the box to closed
}

void openBox() // open the box
{
  rc_servo_set(0, 80); // set the servo to 0 degrees
  isOpen = 1; // set the state of the box to open
}

void closeBox() // close the box
{
  rc_servo_set(0, 0); // set the servo to 180 degrees
  isOpen = 0; // set the state of the box to closed
}

void boxControllerTask(void *pvParameters) // the task that controls the box
{
  for(;;)
  {
    EventBits_t eventBits = xEventGroupWaitBits(dataReadyEventGroup,ALL_READY_BITS,pdTRUE,pdTRUE,portMAX_DELAY);
    if(eventBits & (ALL_READY_BITS)) // if all the data is ready
    {
      if( xSemaphoreTake (configMutex, (TickType_t) 50) == pdTRUE) // Check if mutex is available
      {
        openBox(); // open the box
        printf("\nBox opened\n");
      }
      else // if the mutex is not available
      {
        if(isOpen == 1) // if the box is open
        {
          closeBox(); // close the box
          printf("\nBox closed\n");
        }
      }
      xSemaphoreGive(configMutex); // give the mutex back
    }
    else // if the data is not ready
    {
      printf("\nShared resources not ready --> configMutex\n");
    }
    vTaskDelay(pdMS_TO_TICKS(5000)); // delay the task for 5 seconds
  }
}

void createBoxControllerTask(UBaseType_t TaskPriority) // create the task that controls the box
{
  xTaskCreate(
    boxControllerTask, // the task that controls the box
    "boxControllerTask", // name of the task
    configMINIMAL_STACK_SIZE, // stack size
    NULL, // no parameters
    tskIDLE_PRIORITY + TaskPriority, // priority
    NULL // no handle
  );
}
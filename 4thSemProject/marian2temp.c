#include <hih8120.h>

uint16_t temperature = 0; // variable to store the temperature value
uint16_t humidity = 0; // variable to store the humidity value

uint16_t getTemperature() {
  return temperature; // return the temperature value
}

uit16_t getHumidity() {
  return humidity; // return the humidity value
}

void initializeTempAndHumidityDriver() {
  hih8120_driverReturnCode_t returnCode = HIH8120_initialise();   // create a variable to store the return code
  if ( HIH8120_OK == returnCode ) // check if the return code is OK
    {
    printf("HIH8120 initialised\n");
    }
  else
  {
    printf("HIH8120 initialisation failed\n");
  }
 
}

void measureTempAndHumidityAndWakeUp()
{
  hih8120DriverReturnCode_t returnCode = hih8120_wakeup(); //create a variable to store the return code
  if( HIH8120_OK == returnCode )
  {
    printf("HIH8120 woken up\n");
    delay(50);
    hih8120DriverReturnCode_t returnCode2 = hih8120_measure();
    if ( HIH8120_OK = returnCode2 )
    {
      temperature = hih8120_getTemperature();
      humidity = hih8120_getHumidity();
    }
  }
  else
  {
    printf("HIH8120 wakeup failed\n");
  }
}






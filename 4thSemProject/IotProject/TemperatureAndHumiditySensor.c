#include <hih8120.h>
#include <TemperatureAndHumiditySensor.h>

uint16_t temperature = 0;
uint16_t humidity = 0;

uint16_t getTemperature() {
    return temperature;
}

uit16_t getHumidity() {
    return humidity;
}
// ******** Function to initialize the sensor ********
void initializeTempAndHumidityDriver() {
    hih8120_driverReturnCode_t returnCode = HIH8120_initialise();
    if ( HIH8120_OK == returnCode ) 
    {
        printf("HIH8120 initialised\n");
    }
    else
    {
        printf("HIH8120 initialisation failed\n");
    }
 
}


// ******* Function to wake up the driver and measure temperature and humidity ********
void measureTempAndHumidity()
{
    hih8120DriverReturnCode_t returnCode = hih8120_wakeup();
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


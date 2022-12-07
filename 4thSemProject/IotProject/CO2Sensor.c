#include "CO2Sensor.h"

uint16_t co2PPM = 0;
mh_z19_returnCode_t rc;

uint16_t getCO2()
{
    return co2PPM;
}


void myCO2CallBack(uint16_t ppm)
{
    co2PPM = ppm;
}

void initializeCO2Driver()
{
    mh_z19_initialise(ser_USART3); // Initialize MH-Z19
    mh_z19_injectCallBack(myCo2CallBack); // inject the callback function
}

void measureCO2()
{
    rc = mh_z19_takeMeassuring();
    if(rc != MHZ19_OK)
    {
        printf("Error: %d");
    }
}
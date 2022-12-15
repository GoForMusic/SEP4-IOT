
#include "Configuration.h"



uint16_t co2BenchMark; // CO2 benchmark value
uint16_t tempBenchMark; // Temperature benchmark value
uint16_t humBenchMark; // Humidity benchmark value

//Mutex handle
SemaphoreHandle_t configMutex;


void createConfiguration() // function to create the configuration and set benchmark values
{
    co2BenchMark = 1000; 
    tempBenchMark = 50;
    humBenchMark = 1000;
    configMutex = xSemaphoreCreateMutex();
    xSemaphoreGive(configMutex);
}


uint16_t getCo2BenchMark() // Get CO2 benchmark value
{
    return co2BenchMark;
}
uint16_t getTempBenchMark() // get temperature benchmark value
{
    return tempBenchMark;
}
uint16_t getHumBenchMark() // get humidity benchmark value
{
    return humBenchMark;
}


void setCo2BenchMark(uint16_t benchMark) // setter for co2 benchmark value
{
    co2BenchMark = benchMark;
}
void setTempBenchMark(uint16_t benchMark) // setter for temo benchmark value
{
    tempBenchMark = benchMark;
}
void setHumBenchMark(uint16_t benchMark) // setter for humidity benchmark value
{
    humBenchMark = benchMark;
}
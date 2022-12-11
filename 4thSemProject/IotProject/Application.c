 #include "CO2Sensor.h"
 #include "TemperatureAndHumiditySensor.h"
 #include "SensorDataPackageHandler.h"
 
 UBaseType_t taskTempHumPriority=3;
 UBaseType_t taskCo2Priority=4;
 
 void createTask()
 {
 	for(;;)
 	{
 		create(taskTempHumPriority);
 		createCo2Task(taskCo2Priority);
 		setHumidityX10Percent(getHumidity());
 		setTemperatureX10C(getTemperature());
 		setCos2Ppm(getCo2());
 	}
 	
 }
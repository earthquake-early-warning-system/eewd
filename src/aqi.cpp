#include <ConfigurableSoftwareSerial.h>
#include "CalculateAQI.h"
#include <PMS.h>
#include "aqi_config.h"


ConfigurableSoftwareSerial pmsSerial(PIN_PMS_TX, PIN_PMS_RX, false, 256);
PMS pms(pmsSerial);
PMS::DATA pms_data;

SensorData sensorData;

 

#define UART_BAUD               9600
#define UART_STOP_BITS          2
#define UART_PARITY             'N'
#define UART_DATA_BITS          7
 

void setup_aqi()
{
    pmsSerial.begin(UART_BAUD);//, UART_STOP_BITS, UART_PARITY, UART_DATA_BITS);
}

void resetSensorAvg()
{  
  sensorData = {
    PM_AE_UG_1_0: 0,
    PM_AE_UG_2_5: 0,
    PM_AE_UG_10_0: 0,
    AQI: 0,
    numReads: 0  
  };
}
void loop_aqi()
{
  
  
 if (pms.read(pms_data)) {   

    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(pms_data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(pms_data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(pms_data.PM_AE_UG_10_0);

    

    // calculate AQI
    float aqi = CalculateAQI::getPM25AQI(pms_data.PM_AE_UG_2_5);

    // update for averaging
    CalculateAQI::updateSensorData(sensorData, pms_data, aqi);

    SensorData averagedData = CalculateAQI::getAveragedData(sensorData);

    Category cat = CalculateAQI::getCategory(averagedData.AQI);



    Serial.print("PM 1.0 (ug/m3) avg: ");
    Serial.println(averagedData.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3) avg: ");
    Serial.println(averagedData.PM_AE_UG_2_5);
 
    Serial.print("PM 10.0 (ug/m3) avg: ");
    Serial.println(averagedData.PM_AE_UG_10_0);

    Serial.print("Dust AQI avg: ");
    Serial.println(averagedData.AQI);
    Serial.println(aqi);
    Serial.println(cat.level);
    Serial.println(cat.color);
 
    resetSensorAvg();

    Serial.println();
  }

  

}

 
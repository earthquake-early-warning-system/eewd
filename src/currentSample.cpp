
#include <Arduino.h>
#include "config.h"
//#include "version.h"

#include "elapsedMillis.h"

elapsedMillis analogSampleUpdateTime;

#include "EmonLib.h"

EnergyMonitor emon1; // Create an instances

float currentSample_Init()
{

  emon1.current(A0, 60.6); // 111.1);             // Current: input pin, calibration.

  emon1.calcIrms(1480); // stabilize the reading
  emon1.calcIrms(1480);
  emon1.calcIrms(1480);
  emon1.calcIrms(1480);
  return emon1.calcIrms(1480);
}

boolean curretSample_Loop(double *Irms)
{
  //
  //Read analog data as per
  // https://learn.openenergymonitor.org/electricity-monitoring/ct-sensors/interface-with-arduino
  //

  //yeild();

  bool status = false;

  //if(analogSampleUpdateTime > 20)
  {
    analogSampleUpdateTime = 0;

    *Irms = emon1.calcIrms(1480); //why ? 1480 samples // Calculate Irms only

    // if(testMotorStatusChange > testTime)
    // {
    //   testMotorStatusChange = 0;
    //   testTime = ESP8266TrueRandom.random(10000,20000);
    //
    //   //gScript_type = hb;
    //   *Irms = ESP8266TrueRandom.random(10,600)/100.0;
    //
    //
    //   Serial.print("Random test ts:");
    //   Serial.print(testTime);
    //   Serial.print(" test Irms: ");
    //   Serial.println(*Irms);
    // }

    if (*Irms > current_factor_threshold_for_motor_ON) // probably ignore accounting anything below 200mA
    {

      if (*Irms > current_factor_threshold_for_motor_ON)
      {
        status = true; //signifies motor is ON
      }
    }

    if (*Irms < current_factor_threshold_for_motor_ON)
    {

      status = false; //signifies motor is OFF
    }

    /*Serial.print(millis());
    Serial.print(" : ");
    Serial.print(*Irms);
    static float average = 0.0;
    Serial.print(" : ");
    average = approxRollingAverage(average, *Irms);
    Serial.println(average);*/
  }

  return status;
}

#include <Arduino.h>

#include <MedianFilter.h>
MedianFilter samples_curr(5, 30);

unsigned long timer_Irms = 0, timer_millis_Irms = 0;
unsigned long time_profile_Irms = 0;

uint16_t dsr_Irms = 0;
uint32_t sr_Irms = 0, lsr_Irms=0;

double Irms, Irms_filtered ; 

boolean curretSample_Loop(double *Irms);
float currentSample_Init();

void Irms_resetSampleTimer()
{
  timer_Irms = millis();
}

float Irms_getCurr()
{
  return Irms;
}

float Irms_getFilteredCurr()
{
  return Irms_filtered;
}

void Irms_setup() {
  //Serial.begin(115200);
  // put your setup code here, to run once:
  double Irms = currentSample_Init();
}

bool Irms_loop() {
  // put your main code here, to run repeatedly:
  bool state;
  

  if(millis() - timer_millis_Irms > 400)
  {
    timer_millis_Irms = millis();
    time_profile_Irms = micros();
    state = curretSample_Loop(&Irms);
    sr_Irms++;

    yield();
    samples_curr.in((int)((float)Irms * 1000.0));
    Irms_filtered = ((float)samples_curr.out()) * 0.001;

    time_profile_Irms = micros() - time_profile_Irms;

  }
   
  if(millis() - timer_Irms > 1000)
  {
    dsr_Irms = sr_Irms-lsr_Irms;
    timer_Irms = millis(); 
    //
    // https://www.zomato.com/ncr/taste-of-gurgaon-sector-83-gurgaon/order/qzRqAEdE

    Serial.print ("| IRMS | ");
    Serial.print ("dt ");
    Serial.print(time_profile_Irms);
    Serial.print(" dsr ");
    Serial.print(dsr_Irms);
    Serial.print (" Irms f ");
    Serial.print(Irms_filtered );
    Serial.print (" Irms ");
    Serial.println (Irms );

    lsr_Irms = sr_Irms;

  } 


  return state;

 
}

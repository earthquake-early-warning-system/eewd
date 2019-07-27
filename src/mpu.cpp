#include <Arduino.h>

#include <Wire.h>
 

#include <MedianFilter.h>
MedianFilter samples_acc_mpu(3, 0); // devide by 10000 as targetting 0.100
MedianFilter samples_temp_mpu(3, 25000);  // devide by 100 as targetting 35.0

#include "arduinoFFT.h"

#include "config.h"

#include "common_def.h"


arduinoFFT FFT_mpu = arduinoFFT();/* Create FFT object */
/*
These values can be changed in order to evaluate the functions
*/

const uint16_t samples_mpu = 32; //This value MUST ALWAYS be a power of 2
const float mag_multiflier = 10000.0; // factor

// This should be minimum just double of the max target frequency.
// I chose 4 times
const long sampling_duration_us = 80000; // Sampling frequency 1000s/80ms = 12.5Hz max estimatable freq
const long number_of_samples_per_iteration = samples_mpu + 7; // + 7 is for error

const long time_division = number_of_samples_per_iteration * sampling_duration_us/1000; //ms // Total Sampling duration

// Since this was only tested
bool log_scale_on = false;//(samples_mpu==256) && (mag_multiflier==100000.0f) && (time_division==10000) && (sampling_duration_us == 40000);

/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/
double vReal_mpu[samples_mpu];
double vImag_mpu[samples_mpu];


#include "I2CScanner.h"

I2CScanner scanner_mpu;

#define OFFSET_COUNT (10)
float mpu_offset = 0.0f;
bool has_offset_calculate = false;
int mpu_offset_sample_count = 0;
float mpu_offset_samples[OFFSET_COUNT]={0.0f};
 
void setup_mpu();
void loop_mpu();

float Am_mpu=0.0, temp_mpu=0.0, acc_fft_magnitude_mpu=0.0, acc_fft_magnitude_filtered_mpu=0.0, temp_filtered_mpu=0.0;

float mpu_getAccelMag()
{
  return Am_mpu;
}

const float min_log_v = 0.001f,  max_log_v = 20.0f;

float getLogScale(double val_to_scale)
{
  float d = val_to_scale;
  if(true == log_scale_on)
  {
    //f(x) = log(1+x)/log(1+max)
    if( (float(val_to_scale)>min_log_v) && (float(val_to_scale)<min_log_v) )
    {
      d = logf(val_to_scale+1)/logf(1+max_log_v);
    }
    else
    {
      d = val_to_scale;
    }
    
  }

  return d;

}

float mpu_getAccelFftMag()
{ 
  return acc_fft_magnitude_mpu;
}

float mpu_getAccelFftMagFiltered() 
{
  return acc_fft_magnitude_filtered_mpu;
}

float mpu_getTemp() 
{
  return temp_mpu;
}

float mpu_getTempFiltered() 
{
  return temp_filtered_mpu;
}

unsigned long timer_mpu = 0, timer_micros_mpu = 0;
unsigned long time_profile_mpu = 0;

void mpu_resetSampleTimer()
{
  timer_mpu = millis();
}

bool mpu_setup() {
  Serial.begin(115200);

  setup_mpu();

  bool status = scanner_mpu.Scan(); // Followed by wire.begin
 
  mpu_offset = 0.0f;
  
  return status;
 
}

uint16_t dsr_mpu = 0;
uint32_t sr_mpu = 0, lsr_mpu=0;
int acc_vreal_index_mpu = 0;

double valid_frequency_mpu=0.0;

void mpu_loop() {
  //mpu6050.update();

  if(micros() - timer_micros_mpu > sampling_duration_us)
  {
    timer_micros_mpu = micros();
    time_profile_mpu = micros();
    loop_mpu();
    if(samples_mpu>acc_vreal_index_mpu)
    { 
      vReal_mpu[acc_vreal_index_mpu] = Am_mpu*mag_multiflier;
      vImag_mpu[acc_vreal_index_mpu] = 0.0;
      acc_vreal_index_mpu++;
    }
    sr_mpu++;
    time_profile_mpu = micros() - time_profile_mpu;
  } 

  if(millis() - timer_mpu > time_division)
  {
    
    timer_mpu = millis();

    dsr_mpu = sr_mpu-lsr_mpu; /* Sampling frequency */
    double samplingFrequency = (double)dsr_mpu/((double)time_division/1000.0); // change to second

    FFT_mpu.Windowing(vReal_mpu, samples_mpu, FFT_WIN_TYP_HAMMING, FFT_FORWARD);/* Weigh data */
    FFT_mpu.DCRemoval();
    FFT_mpu.Compute(vReal_mpu, vImag_mpu, samples_mpu, FFT_FORWARD); /* Compute FFT */
    FFT_mpu.ComplexToMagnitude(vReal_mpu, vImag_mpu, samples_mpu); /* Compute magnitudes */
    
    double x;
    double v;
    FFT_mpu.MajorPeak(vReal_mpu, samples_mpu, samplingFrequency, &x, &v);

    valid_frequency_mpu = 0.0;
    valid_frequency_mpu = (int)(v/mag_multiflier) < (int)0.25 ? valid_frequency_mpu : x; // it should be more than 1dB.

    acc_fft_magnitude_mpu = ((v)) ; // for external use keeping 
    acc_fft_magnitude_mpu = getLogScale(acc_fft_magnitude_mpu);
 
    acc_fft_magnitude_mpu = (acc_fft_magnitude_mpu/mag_multiflier - mpu_offset);
    acc_fft_magnitude_mpu = acc_fft_magnitude_mpu < 0 ? acc_fft_magnitude_mpu*-1.0 : acc_fft_magnitude_mpu;

    acc_fft_magnitude_filtered_mpu = samples_acc_mpu.in((int)((float)acc_fft_magnitude_mpu*mag_multiflier)); // already x1000 for magnitude
    acc_fft_magnitude_filtered_mpu = acc_fft_magnitude_filtered_mpu / mag_multiflier;

    if(has_offset_calculate == false)
    {
      mpu_offset_samples[mpu_offset_sample_count%OFFSET_COUNT] = acc_fft_magnitude_mpu;
      mpu_offset_sample_count++;
    

      if(mpu_offset_sample_count == OFFSET_COUNT)
      {
          float offset_local = 0;
          for(int i=0; i< OFFSET_COUNT; i++)
          {
            offset_local += mpu_offset_samples[i];
          }

          offset_local /= OFFSET_COUNT;

          mpu_offset = offset_local;

          has_offset_calculate = true;


          sprintf(print_buffer, "| mpu | vib offset %f", mpu_offset);
          Serial.println(print_buffer);

          syslog_debug(print_buffer); 
      }

      acc_fft_magnitude_mpu = 0.0f;
      acc_fft_magnitude_filtered_mpu = 0.0f;

    }
 
  
    samples_temp_mpu.in((int)((float)temp_mpu*1000.0)); // x1000 for magnitude
    temp_filtered_mpu = ((float)samples_temp_mpu.out()) * 0.001; // / by 1000
    
    for (size_t i = 0; i < samples_mpu;++i) 
    {
      vReal_mpu[i] = 0.0;
      vImag_mpu[i] = 0.0;
    }

    acc_vreal_index_mpu = 0;

    sprintf(print_buffer, "| MPU | dt %d smpl %d %2.4f Hz %2.3f dB %2.1f", time_profile_mpu, dsr_mpu, valid_frequency_mpu, acc_fft_magnitude_mpu, temp_mpu );
    syslog_debug(print_buffer);
    
    Serial.print("| MPU | ");
    Serial.print("dt ");
    Serial.print(time_profile_mpu);
    Serial.print(" dsr ");
    Serial.print(dsr_mpu);
    Serial.print(" Am ");
    Serial.print(Am_mpu,4);

    Serial.print(" res ");

    // number of sample per second for nyquist =  (dsr_mpu / (total sample duration) ) / 2
    //   
    float temporary_time = ((double)time_division/1000.0f); // The second
    float total_samples = dsr_mpu/temporary_time ;
    float nyquist_samples = total_samples * 0.5; 
    Serial.print((nyquist_samples)/(samples_mpu*0.5),4); // (samples in a second divide by 2) / (fft points/2)
 
    Serial.print(" Hz/bin [ Am freq ");
    Serial.print(valid_frequency_mpu,4);
    Serial.print(" Hz, mag ");
    Serial.print(acc_fft_magnitude_mpu);

    Serial.print(" dB ] Am filtered ");
    Serial.print(acc_fft_magnitude_filtered_mpu,4);

    Serial.print(" Tm ");
    Serial.print(temp_mpu,2);
    
    Serial.print(" Tm filtered ");
    Serial.println(temp_filtered_mpu,2);
    
    lsr_mpu = sr_mpu;
  }

    
    // Serial.println("=======================================================");
    // Serial.print("temp : ");Serial.println(mpu6050.getTemp());
    // Serial.print("accX : ");Serial.print(mpu6050.getAccX());
    // Serial.print("\taccY : ");Serial.print(mpu6050.getAccY());
    // Serial.print("\taccZ : ");Serial.println(mpu6050.getAccZ());
  
    // Serial.print("gyroX : ");Serial.print(mpu6050.getGyroX());
    // Serial.print("\tgyroY : ");Serial.print(mpu6050.getGyroY());
    // Serial.print("\tgyroZ : ");Serial.println(mpu6050.getGyroZ());
  
    // Serial.print("accAngleX : ");Serial.print(mpu6050.getAccAngleX());
    // Serial.print("\taccAngleY : ");Serial.println(mpu6050.getAccAngleY());
  
    // Serial.print("gyroAngleX : ");Serial.print(mpu6050.getGyroAngleX());
    // Serial.print("\tgyroAngleY : ");Serial.print(mpu6050.getGyroAngleY());
    // Serial.print("\tgyroAngleZ : ");Serial.println(mpu6050.getGyroAngleZ());
    
    // Serial.print("angleX : ");Serial.print(mpu6050.getAngleX());
    // Serial.print("\tangleY : ");Serial.print(mpu6050.getAngleY());
    // Serial.print("\tangleZ : ");Serial.println(mpu6050.getAngleZ());
    //Serial.println("=======================================================\n");
    //scanner.Scan();
    //Serial.println("=======================================================\n");
    
    
    
  //}

}


// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain 
const int MPU_addr=0x68;  // I2C address of the MPU-6050
void setup_mpu()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  //Serial.begin(9600);
}
void loop_mpu()
{
  int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  // 16384 @ 2g default settings
  // 2.0/32768.0 mG

  float Ax = (float)AcX*2.0/32768.0;
  float Ay = (float)AcY*2.0/32768.0;
  float Az = (float)AcZ*2.0/32768.0;

  Am_mpu = sqrtf(Ax*Ax + Ay*Ay + Az*Az);

  if (isnan(Am_mpu))
  {
    Am_mpu = 0.0;
  }

  
  temp_mpu = (Tmp/340.00+36.53);
  //Serial.print("Acm = "); Serial.print(Am, 4);
  //Serial.print(" | AcX = "); Serial.print(Ax, 4);
  //Serial.print(" | AcY = "); Serial.print(Ay, 4);
  //Serial.print(" | AcZ = "); Serial.print(Az, 4);
  //Serial.print(" | Tmp = "); Serial.println(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  //Serial.print(" | GyX = "); Serial.print(GyX);
  //Serial.print(" | GyY = "); Serial.print(GyY);
  //Serial.print(" | GyZ = "); Serial.println(GyZ);
  //delay(333);
}
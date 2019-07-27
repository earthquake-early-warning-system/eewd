//#include <FS.h> //this needs to be first, or it all crashes and burns...

// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include <Arduino.h>
#include "config.h"
//#include "version.h"
#include <ESP8266WiFi.h>

#include "RemoteDebug.h"
RemoteDebug Debug;

#include "common_def.h"



#ifdef ESP8266
extern "C"
{
#include "user_interface.h"
}
#endif

#include "elapsedMillis.h"

const char *HOST_NAME = "remotedebug-air_conditioner_energy";

elapsedSeconds checkTelnetTime, checkPrintTime;
elapsedMillis checkThingSpeakTime;
unsigned long last_time_thingspoke, last_time_telnet_talked;
const int updateTelnetInterval = 1; // * 1000;

//WiFiClient client;

IPAddress local_IP(192, 168, 43, 152);
IPAddress gateway(192, 168, 43, 255);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8); // Google DNS

bool whether_post_wifi_connect_setup_done;

// //=======================================================================
void ICACHE_RAM_ATTR onTimerISR()
{
  //handleClients();
  timer1_write(5000); //12us??
}


bool is_safe_mode_active= false;
bool has_config_received = false;

void checkResetCause()
{
  Serial.println(__LINE__);

    rst_info * rst_inf = ESP.getResetInfoPtr();


    Serial.println(__LINE__);
    syslog_info((char*)ESP.getResetReason().c_str());
    
    Serial.println(__LINE__);

    if(rst_inf->reason==REASON_EXCEPTION_RST)
    {
      is_safe_mode_active = true;
      syslog_info("Non zero reset reason. Going in safe mode.");
      //in case there was some code issue
      return;
    }

    Serial.println(__LINE__);
}

void setup()
{
  Serial.begin(115200);

  //WiFi.disconnect();
  //delay(10);
  //WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  //WiFi.config(local_IP, gateway, subnet, dns);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to configured wifi...");
  syslog_info("Connecting to configured wifi...");

  wifimanager_setup();
 
  checkResetCause();

  mpu_setup();

  pinMode(LED_BUILTIN, OUTPUT);

  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   Serial.print(".");
  //   delay(1000);
  // }

  // Serial.print("Wifi connection OK ");
  // Serial.printf("IP %s\n", WiFi.localIP().toString().c_str());

  whether_post_wifi_connect_setup_done = false;

  //setup_server();
  // handleClients();
 
  //Serial.printf("Version %s\n",_VER_);
  Serial.printf("Build at %s %s\n", __DATE__, __TIME__);
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  Serial.print("Version: ");
  Serial.println(_VER_);
  syslog_info("Version");
  syslog_info(_VER_);


  /* For Device's unique ID */
  uint8_t mac[6];
  wifi_get_macaddr(STATION_IF, mac);

  DEVICE_ID[0] = mac[0];
  DEVICE_ID[1] = mac[1];
  DEVICE_ID[2] = mac[2];
  DEVICE_ID[3] = mac[3];
  DEVICE_ID[4] = mac[4];
  DEVICE_ID[5] = mac[5];


  setDeviceMacStr();
  // String mac_str = (WiFi.macAddress());
  // mac_str.replace(":", "");
  //strncpy(getDeviceIDstr(), mac_str.c_str(), 13);

  String device_id_based_ssid = "Device hotspot can be EEWD_" + String(getDeviceMacStr());
  Serial.println(device_id_based_ssid);
  syslog_info(( char*)device_id_based_ssid.c_str());

#if (CURRENT_SUB_DEVICE == ENABLED)
  Irms_setup();

  // For complete sampling
  // Without this the first sample after this point is incomplete.
  Irms_resetSampleTimer();
#endif

#if (VIBRATION_SUB_DEVICE == ENABLED)
  mpu_resetSampleTimer();
#endif

  //sendDeviceId(); // To be worked on insert_data.php file for this. t can create a table automatically if not existing

  // timer1_attachInterrupt(onTimerISR);
  // timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE); // TIM_LOOP
  // timer1_write(5000); // 1ms hope fully //120000 us

  //timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  //timer1_write(5000000);//1 second
}

bool last_state = false;
unsigned long sr, ts_acc;


void loop()
{

  unsigned long ts = millis(), dt_acc, dt_loop = micros();

  double Irms = 0, Irms_filtered = 0;
  float temp = 0, temp_filtered = 0; // readTemp();
  float acc = 0, acc_filtered = 0;

  acc = 0;

  if (false == whether_post_wifi_connect_setup_done)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      setup_OTA();
      //rd_setup(HOST_NAME);
      whether_post_wifi_connect_setup_done = true;
    }
  }
  if (true == whether_post_wifi_connect_setup_done)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      if (loop_OTA())
      {
        return;
      }
      //rd_loop();
    }
  }

  if(is_safe_mode_active==true)
  {
    return;
  }

  if(has_config_received == false)
  {
      sprintf(getPrintBuffer(), "No device config found. Synching...");
      Serial.println(getPrintBuffer());
      syslog_debug(getPrintBuffer());
      has_config_received = setup_php_server();
      delay(1000);
      return;
  }

  bool config_proc_st = processConfig();
  if(config_proc_st==true)
  {
    delay(0);
    
    
    sprintf(getPrintBuffer(), "code updated resetting...");
    Serial.println(getPrintBuffer());
    syslog_debug(getPrintBuffer());

    delay(1000);

    ESP.reset();
  }

  ts = millis();

#if (VIBRATION_SUB_DEVICE == ENABLED)
  mpu_loop();

  temp = mpu_getTemp();
  acc = mpu_getAccelFftMag();
  temp_filtered = mpu_getTempFiltered();
  acc_filtered = mpu_getAccelFftMagFiltered();
#endif

#if (CURRENT_SUB_DEVICE == ENABLED)
  bool state = Irms_loop(); // It is not measuring status

  Irms = Irms_getCurr();

  Irms_filtered = Irms_getFilteredCurr();
#endif

  // Irms, Irms_filtered, temp, temp_filtered, acc, acc_filtered

  ts = millis() - ts;
  dt_loop = micros() - dt_loop;

  // if (checkPrintTime > 0)
  // {
  //   checkPrintTime = 0;
  //   Serial.printf("* loop dt %d  uptm %d(VERBOSE)\n", dt_loop, millis());
  // }

  if (true == whether_post_wifi_connect_setup_done)
  {
    if (checkTelnetTime >= updateTelnetInterval)
    {
      checkTelnetTime = 0;
      //last_time_telnet_talked = millis();
      sprintf_P(print_buffer, "* %f (%f) A, %f (%f) dC, %f (%f) G(VERBOSE)\n", Irms, Irms_filtered, temp, temp_filtered, acc, acc_filtered);
      //DEBUG_V(print_buffer);//"* %f (%f) A, %f (%f) dC, %f (%f) G(VERBOSE)\n", Irms, Irms_filtered, temp, temp_filtered, acc, acc_filtered);
      //syslog_debug(print_buffer);
    }
  }
  if (checkThingSpeakTime > updateThingSpeakInterval) // && samples.getCount() == samples.getSize())
  {
    checkThingSpeakTime = 0;
    //last_time_thingspoke = millis();
    sprintf(print_buffer, "data sending time");
    Serial.println(print_buffer);
    syslog_info(print_buffer);

    long time_wifi_check = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(250);

      if (millis() - time_wifi_check > 60000)
      {
        Serial.println("Resetting the device as not connecting to configured wifi settings...");
        delay(2000);
        Serial.println("Repeat: Resetting the device as not connecting to configured wifi settings...");
        delay(2000);
        Serial.println("Repeat: Resetting the device as not connecting to configured wifi settings...");
        delay(2000);
        ESP.reset();
      }
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      sr++;
      loop_php_server(sr, millis(), temp_filtered, temp, Irms_filtered, Irms, acc_filtered, acc);

      sprintf(print_buffer, "Wifi connection OK - IP %s", WiFi.localIP().toString().c_str());
      Serial.println();
      Serial.println(print_buffer);
      syslog_info(print_buffer);

      //Serial.print("\nWifi connection OK ");
      //Serial.printf("IP %s\n", WiFi.localIP().toString().c_str());

      // For complete sampling
      // Without this the first sample after this point is incomplete.
      // These will not affect the sending time

#if (CURRENT_SUB_DEVICE == ENABLED)
      Irms_resetSampleTimer();
#endif

#if (VIBRATION_SUB_DEVICE == ENABLED)
      mpu_resetSampleTimer();
#endif

      return;

      // Working example
      // if (client.connect(server, 80))
      // {

      //   String tsData = apiWritekey;
      //   tsData += "&field1=";
      //   tsData += String(Irms_filtered); //
      //   tsData += "&field2=";
      //   tsData += String(temp_filtered); //
      //   tsData += "&field3=";
      //   tsData += String(acc_filtered);
      //   tsData += "&field4=";
      //   tsData += String(millis() * 0.001);
      //   tsData += "&field5=";
      //   tsData += String(Irms);
      //   tsData += "&field6=";
      //   tsData += String(temp); //
      //   tsData += "&field7=";
      //   tsData += String(acc); //
      //   tsData += "\r\n\r\n";

      //   client.print("POST /update HTTP/1.1\n");
      //   client.print("Host: api.thingspeak.com\n");
      //   client.print("Connection: close\n");
      //   client.print("X-THINGSPEAKAPIKEY: " + String(apiWritekey) + "\n");
      //   client.print("Content-Type: application/x-www-form-urlencoded\n");
      //   client.print("Content-Length: ");
      //   client.print(tsData.length());
      //   client.print("\n\n");
      //   client.print(tsData);
      //   Serial.println("ThingSpeak data sent");
      //   delay(250);
      // }
      // else
      // {
      // }
      //client.stop();
    }
    else
    {
      //WiFi.begin(ssid, password);
    }
  }
}

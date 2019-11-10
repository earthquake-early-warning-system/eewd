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

elapsedSeconds checkMPUStatus;
elapsedSeconds checkTelnetTime, checkPrintTime;
elapsedMinutes checkForcedDataSendTime, safe_mode_timeout_ell;
elapsedMillis checkThingSpeakTime;
//elapsedMillis check_notification_update_time;
unsigned long last_time_thingspoke, last_time_telnet_talked;
const int updateTelnetInterval = 1; // * 1000;

//WiFiClient client;

IPAddress local_IP(192, 168, 43, 152);
IPAddress gateway(192, 168, 43, 255);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8); // Google DNS

bool whether_post_wifi_connect_setup_done;

// //=======================================================================
// void ICACHE_RAM_ATTR onTimerISR()
// {
//   //handleClients();
//   timer1_write(5000); //12us??
// }

bool is_safe_mode_active = false;
bool has_config_received = false;

void checkResetCause()
{
  Serial.println(__LINE__);

  rst_info *rst_inf = ESP.getResetInfoPtr();

  Serial.println(__LINE__);
  syslog_info((char *)ESP.getResetReason().c_str());

  Serial.println(__LINE__);

  if (rst_inf->reason == REASON_EXCEPTION_RST)
  {
    is_safe_mode_active = true;
    syslog_info("Non zero reset reason. Going in safe mode.");
    //in case there was some code issue
    return;
  }

  Serial.println(__LINE__);
}
bool status_mpu;
bool whether_in_offline_mode;

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

  jumper_setup();
  whether_in_offline_mode = jumper_offline_mode_status();


  bool status_notify = notifier_ledNotifierSetup();
  notifier_setNotifierState(NOTIFIER_STATES::_0_NOTIFIER_HB_OFFLINE_MODE);

  if (whether_in_offline_mode)
  {

    //WiFi.mode(WIFI_OFF);

    Serial.print("offline mode turned off wifi.");
    syslog_info("offline mode turned off wifi."); // worthless
  }
  else
  {
    Serial.print("Connecting to configured wifi...");
    syslog_info("Connecting to configured wifi...");

    wifimanager_setup();
  }

  checkResetCause();

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
  //Serial.printf_P("Build at %s %s\n", __DATE__, __TIME__);
  Serial.printf_P("MAC: ");
  Serial.printf_P(WiFi.macAddress().c_str());

  Serial.printf_P("Version: ");
  Serial.printf_P(_VER_);
  syslog_info("Version");
  syslog_info(_VER_);

  Serial.flush();
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
  syslog_info((char *)device_id_based_ssid.c_str());

#if (CURRENT_SUB_DEVICE == ENABLED)
  Irms_setup();

  // For complete sampling
  // Without this the first sample after this point is incomplete.
  Irms_resetSampleTimer();
#endif

  Serial.printf_P("setting up MPU ..");
  status_mpu = mpu_setup();
  Serial.println(". done");

#if (VIBRATION_SUB_DEVICE == ENABLED)
  mpu_resetSampleTimer();
#endif

  //Serial.printf_P("status_notify :%d", status_notify);
  snprintf_P(getPrintBuffer(), MAX_PRINT_BUFFER_SIZE, "status_notify :%d", status_notify);
  syslog_warn(getPrintBuffer());
 
  if (status_mpu == false)
  {
    notifier_setNotifierState(NOTIFIER_STATES::_0_NOTIFIER_CODE_ERROR);

    snprintf(getPrintBuffer(), MAX_PRINT_BUFFER_SIZE, "MPU not intialized.");
    Serial.println(getPrintBuffer());
    syslog_warn(getPrintBuffer());
  }

  // Set default
  ConfigListener *config_lstnr = getJsonConfigListenerPtr();

    Device_config *config = config_lstnr->getDeviceConfigPtr();

    // As per config0
    /*
    config.h

    const float sensor_vibration_threshold_normal = 0.4;
    const float sensor_vibration_threshold_alert = 0.1;
    const float sensor_vibration_threshold_warning = 1.0;
    const float sensor_vibration_threshold_critical = 3.0;
    */
    config->sensor_vibration_threshold_normal[0] = default_config__sensor_vibration_threshold_normal;
    config->sensor_vibration_threshold_alert[0] = default_config__sensor_vibration_threshold_alert;
    config->sensor_vibration_threshold_warning[0] = default_config__sensor_vibration_threshold_warning;
    config->sensor_vibration_threshold_critical[0] = default_config__sensor_vibration_threshold_critical;

  //sendDeviceId(); // To be worked on insert_data.php file for this. t can create a table automatically if not existing

  // timer1_attachInterrupt(onTimerISR);
  // timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE); // TIM_LOOP
  // timer1_write(5000); // 1ms hope fully //120000 us

  //timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  //timer1_write(5000000);//1 second
}

bool last_state = false;
unsigned long sr, ts_acc;

bool high_vibration_sensed = false;

void loop()
{
  whether_in_offline_mode = jumper_offline_mode_status();
 
  if (status_mpu == false)
  {
    notifier_setNotifierState(NOTIFIER_STATES::_0_NOTIFIER_CODE_ERROR);

    // snprintf(getPrintBuffer(), MAX_PRINT_BUFFER_SIZE, "MPU not intialized.");
    // Serial.println(getPrintBuffer());
    // syslog_warn(getPrintBuffer());
  }
  else
  {

    if (whether_in_offline_mode)
    {
      notifier_setNotifierState(NOTIFIER_STATES::_0_NOTIFIER_HB_OFFLINE_MODE);
    }
    else
    {
      notifier_setNotifierState(NOTIFIER_STATES::_0_NOTIFIER_HB_PING);
    }
  }
  notifier_ledNotifierLoop();

  unsigned long ts = millis(), dt_acc, dt_loop = micros();

  double Irms = 0, Irms_filtered = 0;
  float temp = 0, temp_filtered = 0; // readTemp();
  float acc = 0, acc_filtered = 0 , acc_dbl_filtered = 0, acc_freq=0.0;

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
        notifier_ledNotifierLoop();
        return;
      }
      //rd_loop();
    }
  } 
  
  if(Serial.available())
  {
    char c = Serial.read();
    if(c=='r')
    {
      ESP.reset();
    }
  }

  if (is_safe_mode_active == true)
  {
    bool whether_safe_mode_timed_out = safe_mode_timeout_ell >= safe_mode_time_out; // 30 minutes

    if(whether_safe_mode_timed_out==true)
    {
      snprintf(getPrintBuffer(), MAX_PRINT_BUFFER_SIZE, "Safe mode timeout. \n Version %s \n Resetting...", _VER_);
      Serial.println(getPrintBuffer());
      syslog_debug(getPrintBuffer());
      delay(250);
      ESP.reset();
    }  
    
    return;
  }

  if (false == whether_in_offline_mode)
  {
    if (has_config_received == false)
    {
      //    if (check_notification_update_time > notification_update_duration+1)
      {
        // being controlled by earlier part
        // check_notification_update_time
        notifier_setNotifierState(NOTIFIER_STATES::_0_NOTIFIER_CODE_ERROR);
      }
      snprintf(getPrintBuffer(), MAX_PRINT_BUFFER_SIZE, "No device config found. Synching...");
      Serial.println(getPrintBuffer());
      syslog_debug(getPrintBuffer());
      has_config_received = true; //setup_php_server();
      //delay(1000);
      return;
    }
  }



  if (status_mpu == false)
  {
    //while(1)
    {
      notifier_setNotifierState(_0_NOTIFIER_CODE_ERROR);
      notifier_ledNotifierLoop();
      delay(0);
    }
  }

  if (false == whether_in_offline_mode)
  {

    // bool config_proc_st = processConfig();
    // if (config_proc_st == true)
    // {
    //   delay(0);

    //   snprintf(getPrintBuffer(), MAX_PRINT_BUFFER_SIZE, "code updated resetting...");
    //   Serial.println(getPrintBuffer());
    //   syslog_debug(getPrintBuffer());

    //   delay(10);

    //   ESP.reset();
    // }
  }

  ts = millis();

#if (VIBRATION_SUB_DEVICE == ENABLED)

  // if (checkMPUStatus >= 1)
  // {
  //   checkMPUStatus = 0;
  //   status_mpu = mpu_scan();
  // }
  mpu_loop();

  temp = mpu_getTemp();
  acc = mpu_getAccelFftMag();
  acc_freq = mpu_getAccelFreq();
  temp_filtered = mpu_getTempFiltered();
  acc_dbl_filtered = mpu_getAccelTwiceFftMagFiltered(); // mpu_getAccelFftMagFiltered
  acc_filtered = mpu_getAccelFftMagFiltered(); 
#endif

#if (CURRENT_SUB_DEVICE == ENABLED)
  bool state = Irms_loop(); // It is not measuring status

  Irms = Irms_getCurr();

  Irms_filtered = Irms_getFilteredCurr();
#endif

  //processConfig();

  // config fixed
  // if (true == whether_in_offline_mode)
  // {
  //   ConfigListener *config_lstnr = getJsonConfigListenerPtr();

  //   Device_config *config = config_lstnr->getDeviceConfigPtr();

  //   // As per config0
  //   config->sensor_vibration_threshold_normal[0] = 0.1;
  //   config->sensor_vibration_threshold_alert[0] = 1.0;
  //   config->sensor_vibration_threshold_warning[0] = 3.0;
  //   config->sensor_vibration_threshold_critical[0] = 5.0;
  // }

  //if (check_sensor_vibration_time > sensor_vibration_update_duration)
  {
    //   check_sensor_vibration_time = 0;

    ConfigListener *config_lstnr = getJsonConfigListenerPtr();

    Device_config *config = config_lstnr->getDeviceConfigPtr();

    if(acc_dbl_filtered > worth_data_sending_sensor_threshold )
    {
      high_vibration_sensed = true;
    }
    else
    {
      high_vibration_sensed = false;
    }
    

    if (acc_dbl_filtered < config->sensor_vibration_threshold_normal[0])
    {
      notifier_setNotifierState(NOTIFIER_STATES::_3_LED_SENSOR_OK);
    }

    // This is not existing as state in config
    if ((acc_dbl_filtered >= config->sensor_vibration_threshold_normal[0]) && (acc_dbl_filtered < default_config__sensor_vibration_threshold_notify))
    {
      notifier_setNotifierState(NOTIFIER_STATES::_3_LED_SENSOR_NOTIFY);
    }

    if ((acc_dbl_filtered >= default_config__sensor_vibration_threshold_notify) && (acc_dbl_filtered < config->sensor_vibration_threshold_alert[0]))
    {
      notifier_setNotifierState(NOTIFIER_STATES::_3_LED_SENSOR_ALERT);
    }

    if ((acc_dbl_filtered >= config->sensor_vibration_threshold_alert[0]) && (acc_dbl_filtered < config->sensor_vibration_threshold_warning[0]))
    {
      notifier_setNotifierState(NOTIFIER_STATES::_4_LED_SENSOR_WARN);
    }

    if ((acc_dbl_filtered >= config->sensor_vibration_threshold_warning[0]) && (acc_dbl_filtered < config->sensor_vibration_threshold_critical[0]))
    {
      notifier_setNotifierState(NOTIFIER_STATES::_4_LED_SENSOR_EMERGENCY);
    }

    if ((acc_dbl_filtered > config->sensor_vibration_threshold_critical[0]))
    {
      notifier_setNotifierState(NOTIFIER_STATES::_4_LED_SENSOR_CRITICAL);
    }
  }

  notifier_ledNotifierLoop();

  // Irms, Irms_filtered, temp, temp_filtered, acc, acc_dbl_filtered

  ts = millis() - ts;
  dt_loop = micros() - dt_loop;

  if (checkThingSpeakTime > updateThingSpeakInterval) // && samples.getCount() == samples.getSize())
  {
    //checkMPUStatus = 1;
    status_mpu = mpu_scan();
    checkThingSpeakTime = 0;

    if (status_mpu == false)
    {
      notifier_setNotifierState(NOTIFIER_STATES::_0_NOTIFIER_CODE_ERROR);

      snprintf(getPrintBuffer(), MAX_PRINT_BUFFER_SIZE, "MPU not intialized.");
      Serial.println(getPrintBuffer());
      syslog_warn(getPrintBuffer());
    }

    if (true == whether_in_offline_mode)
    {
      //WiFi.mode(WIFI_OFF);
      close_all_connections();

      snprintf(print_buffer, MAX_PRINT_BUFFER_SIZE, "offline mode");
      Serial.println(print_buffer);
      syslog_info(print_buffer);

      return;
    }
    //last_time_thingspoke = millis();
    snprintf(print_buffer, MAX_PRINT_BUFFER_SIZE, "data sending time");
    Serial.println(print_buffer);
    syslog_info(print_buffer);

    //long time_wifi_check = millis();
    if (WiFi.status() != WL_CONNECTED)
    {
      close_all_connections();
      notifier_setNotifierState(NOTIFIER_STATES::_1_LED_WIFI_CONN_FAILED);
  
      whether_in_offline_mode = true;
      notifier_setNotifierState(NOTIFIER_STATES::_0_NOTIFIER_HB_OFFLINE_MODE);

      snprintf(print_buffer, MAX_PRINT_BUFFER_SIZE, "offline mode");
      Serial.println(print_buffer);
      syslog_info(print_buffer);
  
      // while (millis() - time_wifi_check > 60000)
      // {
      //   notifier_setNotifierState(NOTIFIER_STATES::_0_NOTIFIER_CODE_ERROR);
      //   Serial.println("Resetting the device as not connecting to configured wifi settings...");
      //   delay(2000);
      //   Serial.println("Repeat: Resetting the device as not connecting to configured wifi settings...");
      //   delay(2000);
      //   Serial.println("Repeat: Resetting the device as not connecting to configured wifi settings...");
      //   delay(2000);
      //   ESP.reset();
      // }
    }
    // else
    // {
    //   whether_in_offline_mode = false;
    //   notifier_setNotifierState(NOTIFIER_STATES::_0_NOTIFIER_HB_PING);

    // }

    if (WiFi.status() == WL_CONNECTED)
    {
      notifier_setNotifierState(NOTIFIER_STATES::_1_LED_WIFI_CONNECTED);

      bool force_data_send = checkForcedDataSendTime >= force_data_send_duration_minutes; // 30 minutes
      static bool first_data_must_be_sent = true;

      if((high_vibration_sensed==true) || (force_data_send==true) || (first_data_must_be_sent==true))
      { 
        first_data_must_be_sent = false;
        checkForcedDataSendTime = 0; 
        sr++;

        bool status_server = loop_pb_server(
          sr, millis()/1000, 
          acc_freq, acc_filtered, acc_dbl_filtered,
          temp_filtered, Irms_filtered
        );
        //bool status_server = loop_php_server(sr, millis(), temp_filtered, temp, Irms_filtered, Irms, acc_dbl_filtered, acc);

        if(status_server==false)
        {
          whether_in_offline_mode = true;
          notifier_setNotifierState(NOTIFIER_STATES::_0_NOTIFIER_HB_OFFLINE_MODE);
          snprintf(print_buffer, MAX_PRINT_BUFFER_SIZE, "Switched to offline mode");
          Serial.println();
          Serial.println(print_buffer);
          syslog_info(print_buffer);
        } 
        else
        {
          snprintf(print_buffer, MAX_PRINT_BUFFER_SIZE, " Switched to online mode");
          Serial.println(print_buffer);
          syslog_info(print_buffer);
        }

        if(force_data_send==true)
        {
          snprintf(print_buffer, MAX_PRINT_BUFFER_SIZE, " Periodic data sending.");
          Serial.println(print_buffer);
          syslog_info(print_buffer);
        }
      }
      else
      {
        snprintf(print_buffer, MAX_PRINT_BUFFER_SIZE, " Not enough vibration sensed worth sending.");
        Serial.println(print_buffer);
        syslog_info(print_buffer);
      }
      

      

      snprintf(print_buffer, MAX_PRINT_BUFFER_SIZE, "Wifi connection OK - IP %s", WiFi.localIP().toString().c_str());
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
    }
    else
    {
      //WiFi.begin(ssid, password);
    }
  }
}

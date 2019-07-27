#include <FS.h> //this needs to be first, or it all crashes and burns...

#include <Arduino.h>
#include "config.h"

#include "JsonStreamingParser.h"
#include "JsonListener.h"
#include "config_json_stream_parser.h"
 
#include "common_def.h"
 

#include <ESP8266WiFi.h>


JsonStreamingParser parser;
ConfigListener listener;
 

bool whether_post_wifi_connect_setup_done_json_test;

/*
void syslog_debug(char* msg){logger.debug(msg);}
void syslog_info(char* msg){logger.info(msg);}
void syslog_warn(char* msg){logger.warn(msg);}
void syslog_error(char* msg){logger.error(msg);}
 */

void test()
{
  char json[] = "[\
   {\
      \"config_id\":\"0\",\
      \"device_code_to_update_to\":\"v0.0.3-3-g53a0111\",\
      \"device_code_type\":\"DEVT\",\
      \"server_host_address_data\":\"device1-eews.000webhostapp.com\",\
      \"server_host_port_data\":\"80\",\
      \"device_min_code_version\":\"0.0.3-3\",\
      \"device_max_code_version\":\"0.0.3-5\",\
      \"available_sensor_count\":\"3\",\
      \"sensor_current_enabled\":\"0\",\
      \"sensor_temperature_enabled\":\"1\",\
      \"sensor_vibration_enabled\":\"1\",\
      \"sensor_current_threshold_normal\":\"1\",\
      \"sensor_current_threshold_alert\":\"8\",\
      \"sensor_current_threshold_warning\":\"16\",\
      \"sensor_current_threshold_critical\":\"20\",\
      \"sensor_vibration_threshold_normal\":\"0.1\",\
      \"sensor_vibration_threshold_alert\":\"1\",\
      \"sensor_vibration_threshold_warning\":\"3\",\
      \"sensor_vibration_threshold_critical\":\"5\",\
      \"sensor_temperature_threshold_normal\":\"25\",\
      \"sensor_temperature_threshold_alert\":\"35\",\
      \"sensor_temperature_threshold_warning\":\"40\",\
      \"sensor_temperature_threshold_critical\":\"55\",\
      \"sensor_current_sample_time_duration\":\"400000\",\
      \"sensor_vibration_sample_time_duration\":\"80000\",\
      \"sensor_temperature_sample_time_duration\":\"0\",\
      \"sensor_current_total_time_duration\":\"1000\",\
      \"sensor_vibration_total_time_duration\":\"3300\",\
      \"sensor_temperature_total_time_duration\":\"0\",\
      \"sensor_current_buzzer_light_notify_level\":\"255\",\
      \"sensor_temperature_buzzer_light_notify_level\":\"255\",\
      \"sensor_vibration_buzzer_light_notify_level\":\"255\",\
      \"development_print_level\":\"0\"\
   }\
]";
    
    syslog_info(json);
        
    parser.reset();
    for (int i = 0; i < sizeof(json); i++)
    {
        parser.parse(json[i]);
        delay(1);// syslog is dropping log udp packets
    }

    char json2[] = "[ \ 
   {  \
      \"config_id\":\"0\",\
      \"whether_update_available\":\"0\",\
      \"device_code_to_update_to\":\"v0.0.3-3-g53a0111\",\
      \"server_host_address_config\":\"device1-eews.000webhostapp.com\",\
      \"server_host_port_config\":\"80\",\
      \"host_config_server_query_path\":\"\/devices\/get_config.php?\"\
   }\
]";

parser.reset();
syslog_info(json2);
        
    for (int i = 0; i < sizeof(json2); i++)
    {
        parser.parse(json2[i]);
        delay(1);// syslog is dropping log udp packets
    }

}

bool is_safe_mode_active_json= false;

void json_setup()
{
    Serial.begin(115200);

    wifimanager_setup();

    Serial.println("waiting 2 secs");
    syslog_info("waiting 2 secs");
    delay(2000);

    Serial.print("Version: ");
    Serial.println(_VER_);
    syslog_info("Version");
    syslog_info(_VER_);

    String mac_str = (WiFi.macAddress());
    mac_str.replace(":", "");
    strncpy(DEVICE_ID_STR, mac_str.c_str(), 13);

    String device_id_based_ssid = "Device hotspot can be EEWD_" + String(DEVICE_ID_STR);
    Serial.println(device_id_based_ssid);
    syslog_info((char *)device_id_based_ssid.c_str());

    delay(2000);

    Serial.println(__LINE__);
    rst_info * rst_inf = ESP.getResetInfoPtr();

    Serial.println(__LINE__);
    syslog_info((char*)ESP.getResetReason().c_str());
    
    Serial.println(__LINE__);
    if(rst_inf->reason==REASON_EXCEPTION_RST)
    {
      is_safe_mode_active_json = true;
      syslog_info("Non zero reset reason. Going in safe mode.");
      //in case there was some code issue
      return;
    }

    Serial.println(__LINE__); 
    parser.setListener(&listener);  

}


void json_loop() 
{
    if (false == whether_post_wifi_connect_setup_done_json_test)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      setup_OTA();
      //rd_setup(HOST_NAME);
      whether_post_wifi_connect_setup_done_json_test = true;
    }
  }
  if (true == whether_post_wifi_connect_setup_done_json_test)
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

  if(is_safe_mode_active_json==true)
  {
    return;
  }

  test();
}
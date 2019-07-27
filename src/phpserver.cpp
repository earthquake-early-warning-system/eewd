

#include <Arduino.h>
#include "config.h"

#include "JsonStreamingParser.h"
#include "JsonListener.h"
#include "config_json_stream_parser.h"
  
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include "config.h"
 

JsonStreamingParser json_parser;
ConfigListener json_parser_listener;
Device_config * device_config;
 

// extern "C"{
// #include "device_syslog.h"
// };

#include "common_def.h"

float php_tmp_f, php_tmp_r, php_current_f, php_current_r, php_accel_f, php_accel_r;
unsigned long php_sr_ser, php_uptm;

WiFiClient http_wificlient;

//HTTPClient http;

bool getDeviceConfig();
bool readCodeUpdateStatus();
void printDeviceConfig( Device_config *_device_config);

bool setup_php_server()
{

    // Here to get the complete config 



        json_parser.setListener(&json_parser_listener); 
        device_config = json_parser_listener.getDeviceConfigPtr();
 
        bool status = getDeviceConfig() ;

        return status;

        
    //WiFiClient client;

    //if (http.begin(client, php_server))
    {
    }
}

ConfigListener * getJsonConfigListenerPtr()
{
    return &json_parser_listener;
} 

/* [
   {
      "config_id":"0",
      "device_code_to_update_to":"v0.0.3-3-g53a0111",
      "device_code_type":"DEVT",
      "server_host_address_data":"device1-eews.000webhostapp.com",
      "server_host_port_data":"80",
      "device_min_code_version":"0.0.3-3",
      "device_max_code_version":"0.0.3-5",
      "available_sensor_count":"3",
      "sensor_current_enabled":"0",
      "sensor_temperature_enabled":"1",
      "sensor_vibration_enabled":"1",
      "sensor_current_threshold_normal":"1",
      "sensor_current_threshold_alert":"8",
      "sensor_current_threshold_warning":"16",
      "sensor_current_threshold_critical":"20",
      "sensor_vibration_threshold_normal":"0.1",
      "sensor_vibration_threshold_alert":"1",
      "sensor_vibration_threshold_warning":"3",
      "sensor_vibration_threshold_critical":"5",
      "sensor_temperature_threshold_normal":"25",
      "sensor_temperature_threshold_alert":"35",
      "sensor_temperature_threshold_warning":"40",
      "sensor_temperature_threshold_critical":"55",
      "sensor_current_sample_time_duration":"400000",
      "sensor_vibration_sample_time_duration":"80000",
      "sensor_temperature_sample_time_duration":"0",
      "sensor_current_total_time_duration":"1000",
      "sensor_vibration_total_time_duration":"3300",
      "sensor_temperature_total_time_duration":"0",
      "sensor_current_buzzer_light_notify_level":"255",
      "sensor_temperature_buzzer_light_notify_level":"255",
      "sensor_vibration_buzzer_light_notify_level":"255",
      "development_print_level":"0"
   }
]
*/

bool readDeviceConfig()
{ 
    bool status;
 
    status = readCodeUpdateStatus();
      
    if (status == true)
    {
        printDeviceConfig(device_config);
        //*_config = g_device_config_data;
        //printDeviceConfig(&g_device_config_data);
    }
 
    return (device_config->device_id[0] > 0) && status;
 //return readCodeUpdateStatus();
    // It's not mandatory to make a copy, you could just use the pointers
    // Since, they are pointing inside the "content" buffer, so you need to make
    // sure it's still in memory when you read the string

    //return true;
}

// Print the data extracted from the JSON
void printDeviceConfig( Device_config *_device_config_data)
{
    Serial.print("config_id=");
    Serial.println(_device_config_data->config_id[0]);

    Serial.print("whether_update_available=");
    Serial.println(_device_config_data->whether_update_available[0]);

    Serial.print("device_code_to_update_to=");
    Serial.println(_device_config_data->device_code_to_update_to);
    Serial.print("device_code_type=");
    Serial.println(_device_config_data->device_code_type);
    
    Serial.print("server_host_address_data=");
    Serial.println(_device_config_data->server_host_address_data);

    Serial.print("server_host_port_data=");
    Serial.println(_device_config_data->server_host_port_data[0]);

    Serial.print("device_min_code_version=");
    Serial.println(_device_config_data->device_min_code_version);
    Serial.print("device_max_code_version=");
    Serial.println(_device_config_data->device_max_code_version);

    Serial.print("available_sensor_count=");
    Serial.println(_device_config_data->available_sensor_count[0]);
    Serial.print("sensor_current_enabled=");
    Serial.println(_device_config_data->sensor_current_enabled[0]);
    Serial.print("sensor_temperature_enabled=");
    Serial.println(_device_config_data->sensor_temperature_enabled[0]);
    Serial.print("sensor_vibration_enabled=");
    Serial.println(_device_config_data->sensor_vibration_enabled[0]);

    Serial.print("sensor_current_threshold_normal=");
    Serial.println(_device_config_data->sensor_current_threshold_normal[0]);
    Serial.print("sensor_current_threshold_alert=");
    Serial.println(_device_config_data->sensor_current_threshold_alert[0]);
    Serial.print("sensor_current_threshold_warning=");
    Serial.println(_device_config_data->sensor_current_threshold_warning[0]);
    Serial.print("sensor_current_threshold_critical=");
    Serial.println(_device_config_data->sensor_current_threshold_critical[0]);
    Serial.print("sensor_vibration_threshold_normal=");
    Serial.println(_device_config_data->sensor_vibration_threshold_normal[0]);
    Serial.print("sensor_vibration_threshold_alert=");
    Serial.println(_device_config_data->sensor_vibration_threshold_alert[0]);
    Serial.print("sensor_vibration_threshold_warning=");
    Serial.println(_device_config_data->sensor_vibration_threshold_warning[0]);
    Serial.print("sensor_vibration_threshold_critical=");
    Serial.println(_device_config_data->sensor_vibration_threshold_critical[0]);
    Serial.print("sensor_temperature_threshold_normal=");
    Serial.println(_device_config_data->sensor_temperature_threshold_normal[0]);
    Serial.print("sensor_temperature_threshold_alert=");
    Serial.println(_device_config_data->sensor_temperature_threshold_alert[0]);
    Serial.print("sensor_temperature_threshold_warning=");
    Serial.println(_device_config_data->sensor_temperature_threshold_warning[0]);
    Serial.print("sensor_temperature_threshold_critical=");
    Serial.println(_device_config_data->sensor_temperature_threshold_critical[0]);

    Serial.print("sensor_current_sample_time_duration=");
    Serial.println(_device_config_data->sensor_current_sample_time_duration[0]);
    Serial.print("sensor_vibration_sample_time_duration=");
    Serial.println(_device_config_data->sensor_vibration_sample_time_duration[0]);
    Serial.print("sensor_temperature_sample_time_duration=");
    Serial.println(_device_config_data->sensor_temperature_sample_time_duration[0]);
    Serial.print("sensor_current_total_time_duration=");
    Serial.println(_device_config_data->sensor_current_total_time_duration[0]);
    Serial.print("sensor_vibration_total_time_duration=");
    Serial.println(_device_config_data->sensor_vibration_total_time_duration[0]);
    Serial.print("sensor_temperature_total_time_duration=");
    Serial.println(_device_config_data->sensor_temperature_total_time_duration[0]);

    Serial.print("sensor_current_buzzer_light_notify_level=");
    Serial.println(_device_config_data->sensor_current_buzzer_light_notify_level[0]);
    Serial.print("sensor_temerature_buzzer_light_notify_level=");
    Serial.println(_device_config_data->sensor_temperature_buzzer_light_notify_level[0]);
    Serial.print("sensor_vibration_buzzer_light_notify_level=");
    Serial.println(_device_config_data->sensor_vibration_buzzer_light_notify_level[0]);

    Serial.print("development_print_level=");
    Serial.println(_device_config_data->development_print_level[0]); // bit-field
 
    Serial.print("device_id=");
    Serial.println(_device_config_data->device_id[0]); // bit-field
}

//POST /test/demo_form.php HTTP/1.1
//Host: w3schools.com
//name1=value1&name2=value2

bool sendToServer(String data_str, const char *_php_server, uint16_t _php_server_port, const char *_php_server_file_target);

// device_code_type
// congif_id
// code_version my_version
// Device_config g_device_config_data;

bool getDeviceConfig()
{
    // +"&device_code_type="+String(DEVICE_DEVELOPMENT_TYPE);
    // +"&device_code_version="+String(_VER_);
    // +"&device_id=device_id_"+String(DEVICE_ID_STR);
    
    String mac_str = (WiFi.macAddress());
    mac_str.replace(":", "");

    String data_str = "device_code_type=" + String(DEVICE_DEVELOPMENT_TYPE) + "&config_id=" + String(config_id) + "&config_type=l" // long or short
                      + "&device_code_version=" + _VER_
                      + "&Device_mac_id_str="+mac_str;
    bool status = sendToServer(data_str, php_config_server, php_config_server_port, php_config_server_file_target);
    if (status == true)
    {
        status = readDeviceConfig();
    }

    // if (status == true)
    // { 
    //     printDeviceConfig(&g_device_config_data);
    // }

    return status;
}
//  sendToServer(data_str, php_config_server, php_config_server_port, php_config_server_file_target);
 
bool sendToServer(String data_str, const char *_php_server, uint16_t _php_server_port, const char *_php_server_file_target)
{
    bool status = true;
    long total_time_taken = 0;
    long ts_wait_for_client = millis();
    http_wificlient.setTimeout(1000);
    if (!http_wificlient.connect(_php_server, _php_server_port))
    {
        sprintf(print_buffer, "connection failed");
        Serial.println();
        Serial.println(print_buffer);
        syslog_warn(print_buffer);

        status = false;
        return status;
    }

    //Serial.print("'connect' time taken ");
    total_time_taken = millis() - ts_wait_for_client;
    //Serial.println(total_time_taken);
    sprintf(print_buffer, "'connect' time taken %d ms", millis() - ts_wait_for_client);
    //Serial.println();
    Serial.println(print_buffer);
    syslog_debug(print_buffer);

    ts_wait_for_client = millis();

    http_wificlient.setTimeout(50);
    String getStr = "GET " + String(_php_server_file_target) + data_str + " HTTP/1.1\r\nHost: " + String(_php_server) + "\r\n\r\n";

    // Send request to the server:
    http_wificlient.print(getStr);

    sprintf(print_buffer, "http msg %s", getStr.c_str());
    //Serial.println();
    Serial.println(print_buffer);
    syslog_debug(print_buffer);

    getStr = "";

    // Serial.println("===========================");
    // Serial.println(getStr);
    // Serial.println("===========================");

    //Serial.print("'send' time taken ");
    total_time_taken += millis() - ts_wait_for_client;
    //Serial.println(millis() - ts_wait_for_client);

    sprintf(print_buffer, "'send' time taken %d ms", millis() - ts_wait_for_client);
    //Serial.println();
    Serial.println(print_buffer);
    syslog_debug(print_buffer);
    ts_wait_for_client = millis();

    //Serial.print("Wating ... ");
    sprintf(print_buffer, "Wating ... ");
    //Serial.println();
    Serial.println(print_buffer);
    syslog_debug(print_buffer);

    ts_wait_for_client = millis();
    // while (!http_wificlient.available())
    // {
    //     if (millis() - ts_wait_for_client > 10000)
    //     {
    //         //Serial.println(" timed out.");
    //         sprintf(print_buffer, " timed out.");
    //         //Serial.println();
    //         Serial.println(print_buffer);
    //         syslog_warn(print_buffer);
    //         status = false;
    //         http_wificlient.stop();
    //         return status;
    //         break;
    //     }
    // }

    // Check HTTP status
    uint8_t status_str[32] = {0};
    const char *status_ptr = (const char *)status_str;
    http_wificlient.setTimeout(1500);

    http_wificlient.readBytesUntil('\r', status_str, sizeof(status_str));
    // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
    if (strcmp(status_ptr + 9, "200 OK") != 0)
    {
        status = false;
        sprintf(print_buffer, "Unexpected response: %s", status_str);
        //Serial.print(F("Unexpected response: "));
        Serial.println((char *)status_str);
        syslog_warn(print_buffer);
        return status;
    }

    //Serial.print("'wait' time taken ");
    total_time_taken += millis() - ts_wait_for_client;
    //Serial.println(millis() - ts_wait_for_client);
    ESP.wdtFeed();
    sprintf(print_buffer, "'wait' time taken %d ms", millis() - ts_wait_for_client);
    //Serial.println();
    Serial.println(print_buffer);
    syslog_debug(print_buffer);
    ts_wait_for_client = millis();

    //

    // char endOfHeaders[] = "\r\n\r\n";
    // bool ok = http_wificlient.find(endOfHeaders);

    //Serial.println();
    //Serial.print("Data from server: ");

    //Serial.println(http_wificlient.readString());
    // TODO: check for validity
    // call either readReponseContent(...) or read as above

    // Serial.print("'read' time taken ");
    // total_time_taken += millis() - ts_wait_for_client;
    // Serial.println(millis() - ts_wait_for_client);

    // Serial.print("'total' time taken ");
    // Serial.println(total_time_taken);
    sprintf(print_buffer, "'total' time taken %d ms", total_time_taken);
    //Serial.println();
    Serial.println(print_buffer);
    syslog_debug(print_buffer);

    return status;
}

bool whether_new_code_available = false;

bool getUpdateAvailable()
{
    return whether_new_code_available;
}

bool readCodeUpdateStatus()// Device_update_info *_device_update_info)
{
 
    bool status = true;

    sprintf(print_buffer, "Consuming tillend of header...");
    //Serial.println();
    Serial.println(print_buffer);
    syslog_debug(print_buffer);

    //char endOfHeaders[] = "\r\n\r\n";
    //bool ok = http_wificlient.find(endOfHeaders);

    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!http_wificlient.find(endOfHeaders))
    {
        sprintf(print_buffer, "...Invalid response");
        //Serial.println();
        Serial.println(print_buffer);
        syslog_warn(print_buffer);

        return false;
    }
    
    json_parser.reset();

    unsigned long ts_wait_for_client = millis();
    while (http_wificlient.available())
    {
        json_parser.parse( (char) http_wificlient.read() );
        //delayMicroseconds(100);

        if (millis() - ts_wait_for_client > 10000)
        {
            //Serial.println(" timed out.");
            sprintf(print_buffer, " timed out.");
            //Serial.println();
            Serial.println(print_buffer);
            syslog_warn(print_buffer);
            status = false;                                                                                 
            http_wificlient.stop();     
            return false;
            break;
        }
    } 
    
    // // This can use too much of data on internet download part
 
    sprintf(print_buffer, "JSON parsing ended");
    //Serial.println();
    Serial.println(print_buffer);
    syslog_info(print_buffer);

    return true;
}

bool sendDataToServer(String data_str) //, struct Device_config *_config = NULL)
{

    // Device_update_info device_update_info;
    bool status = sendToServer(data_str, php_server, php_server_port, php_server_file_target);
    
    data_str = "";
    
    whether_new_code_available = false;

    if (status == true)
    {
        status = readCodeUpdateStatus();
    }

    if (status == true)
    {
        printDeviceConfig(device_config);
        //*_config = g_device_config_data;
        //printDeviceConfig(&g_device_config_data);
    }
    return status;
}

bool loop_php_server(unsigned long _php_sr, unsigned long _php_uptm, float _php_temp_f, float _php_temp_r, float _php_current_f, float _php_current_r, float _php_accel_f, float _php_accel_r)
{
    //WiFiClient client;

    HTTPClient http;

    php_sr_ser = _php_sr;
    php_uptm = _php_uptm;
    php_tmp_f = _php_temp_f;
    php_tmp_r = _php_temp_r;
    php_current_f = _php_current_f;
    php_current_r = _php_current_r;
    php_accel_f = _php_accel_f;
    php_accel_r = _php_accel_r;

    String query_str = "sr=" + String(php_sr_ser) + "&dt=0" + "&time=0000-00-00T00:00:00" + "&uptm=" + String(php_uptm) + "&temp_filter=" + String(php_tmp_f) + "&temp_raw=" + String(php_tmp_r) + "&curr_filter=" + String(php_current_f) + "&curr_raw=" + String(php_current_r) + "&accel_filter=" + String(php_accel_f) + "&accel_raw=" + String(php_accel_r) 
    + "&device_code_type=" + String(DEVICE_DEVELOPMENT_TYPE) 
    + "&device_code_version=" + String(_VER_) 
    + "&config_id=" +String(device_config->config_id[0])
    + "&config_type=s" // long or short
    + "&device_id="+String(device_config->device_id[0]);
    // crashing Here  => //device_id_" + String(getDeviceIDstr());

    return sendDataToServer(query_str);
}

bool updateCodeUpdateStatus(void)
{
    String query_str = "device_code_type="+String(device_config->device_code_type)
    +"&device_id="+String(device_config->device_id[0]) 
    +"&device_code_version="+String(device_config->device_code_to_update_to);
   
    bool status = sendToServer(query_str, php_upgrade_server, php_upgrade_server_port, php_upgrade_server_status);
    
    if(status == true)
    {
        status = readCodeUpdateStatus();
    }

    if(status == true)
    { 
        status = (device_config->whether_updated[0] == 1);
    }

    return status;

    //sendDataToServer(query_str);


}

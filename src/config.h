#ifndef CONFIG_H
#define CONFIG_H


#include <Arduino.h>
#include <ESP8266WiFi.h>


#define STR(x) #x
#define XSTR(x) STR(x) 
#define _VER_ XSTR(VER)

#define STRCAT(A, B) A B

#define FW_UPDATE_AVAILABLE (1)
//#define FW_UPDATE_UNAVAILABLE (0)

// Four char 
// Development, Observation, Production, Mass-Deployable 
#define DEVICE_DEVELOPMENT_TYPE "DEVT" //  ("OBSN") ("PROD") ("DEPL")
#define UPDATE_PATH "/devices/uploads/"
// Above will be web folder names

 static char DEVICE_ID[6];//={0/* The mac ID */};

 #define MAX_DEVICE_ID_STR_SIZE (14)
 static char DEVICE_ID_STR[MAX_DEVICE_ID_STR_SIZE];//={0/* The mac ID */};
 inline char * getDeviceMacStr()
 {
     return DEVICE_ID_STR;
 }


 #define MAX_PRINT_BUFFER_SIZE (255)
 static char print_buffer[MAX_PRINT_BUFFER_SIZE];// = {0};

 inline char * getPrintBuffer()
 {
     return print_buffer;
 }

 inline void setDeviceMacStr()
 {
     String mac_str = (WiFi.macAddress());
     mac_str.replace(":", "");

     strncpy(DEVICE_ID_STR, mac_str.c_str(), MAX_DEVICE_ID_STR_SIZE < mac_str.length() ? MAX_DEVICE_ID_STR_SIZE : mac_str.length() );
     DEVICE_ID_STR[MAX_DEVICE_ID_STR_SIZE-1] = 0;
     //return DEVICE_ID_STR;
 }

#define MAX_PATH_SIZE (128)
#define MAX_VER_STR_SIZE (32)
#define MAX_SINGLE_LONG_SIZE (1)
#define MAX_MINI_STR_SIZE (8)
#define MAX_SINGLE_CHAR_SIZE (1)

#define SYSLOG_SERVER_ADDRESS ("10.42.0.1") //("192.168.43.71")
#define SYSLOG_DEVICE_NAME ("EEWD")
//#define SYSLOG_DEVICE_IDENTIFICATION ()


//#define ELEM(x, y, z)    x y[z] ;
//ELEM( int, x, 0)
// https://stackoverflow.com/questions/5530248/creating-a-string-list-and-an-enum-list-from-a-c-macro/5530947
#define ENUM_MACRO_DEVICE_CONFIG(name, offset \
,  tv1, v1, asv1 \
,  tv2, v2, asv2 \
,  tv3, v3, asv3 \
,  tv4, v4, asv4 \
,  tv5, v5, asv5 \
,  tv6, v6, asv6 \
,  tv7, v7, asv7 \
,  tv8, v8, asv8 \
,  tv9, v9, asv9 \
,  tv10, v10, asv10 \
,  tv11, v11, asv11 \
,  tv12, v12, asv12 \
,  tv13, v13, asv13 \
,  tv14, v14, asv14 \
,  tv15, v15, asv15 \
,  tv16, v16, asv16 \
,  tv17, v17, asv17 \
,  tv18, v18, asv18 \
,  tv19, v19, asv19 \
,  tv20, v20, asv20 \
,  tv21, v21, asv21 \
,  tv22, v22, asv22 \
,  tv23, v23, asv23 \
,  tv24, v24, asv24 \
,  tv25, v25, asv25 \
,  tv26, v26, asv26 \
,  tv27, v27, asv27 \
,  tv28, v28, asv28 \
,  tv29, v29, asv29 \
,  tv30, v30, asv30 \
,  tv31, v31, asv31 \
,  tv32, v32, asv32 \
,  tv33, v33, asv33 \
,  tv34, v34, asv34 \
,  tv35, v35, asv35 \
,  tv36, v36, asv36 \
,  tv37, v37, asv37 \
,  tv38, v38, asv38 \
,  tv39, v39, asv39 \
,  tv40, v40, asv40 \
,  tv41, v41, asv41 )\
\
    enum name##_enum { v1 =  offset, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37 \
    , v38, v39, v40, v41, vSize};\
    \
    static const char *name##_Strings[] = { #v1, #v2, #v3, #v4, #v5, #v6, #v7, #v8, #v9, #v10, #v11, #v12, #v13, #v14, #v15, #v16, #v17, #v18, #v19, #v20, #v21, #v22, #v23, #v24, #v25, #v26, #v27, #v28, #v29, #v30, #v31, #v32, #v33, #v34, #v35, #v36, #v37 \
    , #v38, #v39, #v40, #v41};\
    \
    inline const char *name##_ToString(int value) { return name##_Strings[value - offset ]; }; \
 \
typedef struct  { \
tv1 v1[asv1]; \
tv2 v2[asv2]; \
tv3 v3[asv3]; \
tv4 v4[asv4]; \
tv5 v5[asv5]; \
tv6 v6[asv6]; \
tv7 v7[asv7]; \
tv8 v8[asv8]; \
tv9 v9[asv9]; \
tv10 v10[asv10]; \
tv11 v11[asv11]; \
tv12 v12[asv12]; \
tv13 v13[asv13]; \
tv14 v14[asv14]; \
tv15 v15[asv15]; \
tv16 v16[asv16]; \
tv17 v17[asv17]; \
tv18 v18[asv18]; \
tv19 v19[asv19]; \
tv20 v20[asv20]; \
tv21 v21[asv21]; \
tv22 v22[asv22]; \
tv23 v23[asv23]; \
tv24 v24[asv24]; \
tv25 v25[asv25]; \
tv26 v26[asv26]; \
tv27 v27[asv27]; \
tv28 v28[asv28]; \
tv29 v29[asv29]; \
tv30 v30[asv30]; \
tv31 v31[asv31]; \
tv32 v32[asv32]; \
tv33 v33[asv33]; \
tv34 v34[asv34]; \
tv35 v35[asv35]; \
tv36 v36[asv36]; \
tv37 v37[asv37]; \
tv38 v38[asv38]; \
tv39 v39[asv39]; \
tv40 v40[asv40]; \
tv41 v41[asv41]; \
    } name ; 
//typedef struct name name;    
    
    /*  \
inline void* getval(const name *ep, int n)\
{ \
    switch(n) \
    { \
      case 1: return ep->config_id; \
      case 2: return ep->whether_update_available; \
      case 3: return ep->device_code_to_update_to; \
      case 4: return ep->device_code_type; \
      case 5: return ep->server_host_address_data; \
      case 6: return ep->server_host_port_data; \
      case 7: return ep->whether_update_available; \
      case 8: return ep->whether_update_available; \
      case 9: return ep->whether_update_available; \
      case 10: return ep->whether_update_available; \
            
    } \
    return -1; \
} 
*/
    
ENUM_MACRO_DEVICE_CONFIG(Device_config, 0, 
  int, config_id, MAX_SINGLE_CHAR_SIZE,
  int, device_id, MAX_SINGLE_CHAR_SIZE,
  unsigned char, whether_updated, MAX_SINGLE_CHAR_SIZE,
 unsigned char, whether_update_available, MAX_SINGLE_CHAR_SIZE,
 char, device_code_to_update_to, MAX_VER_STR_SIZE, // v000.000.000-000-gf676f80
 char, device_code_type, MAX_MINI_STR_SIZE, 
 char , server_host_address_data, MAX_PATH_SIZE,  // e.g. device1-eews.000webhostapp.com
 int , server_host_port_data, MAX_SINGLE_CHAR_SIZE,
 char , host_server_query_path, MAX_PATH_SIZE,
 char , server_host_address_config, MAX_PATH_SIZE, // e.g. device1-eews.000webhostapp.com
 int , server_host_port_config, MAX_SINGLE_CHAR_SIZE,
 char , host_config_server_query_path, MAX_PATH_SIZE,
 char , device_min_code_version,MAX_VER_STR_SIZE, // 000.000.000-000 or full v000.000.000-000-gf676f80
 char , device_max_code_version,MAX_VER_STR_SIZE, // 000.000.000-000 or full v000.000.000-000-gf676f80
 int , available_sensor_count, MAX_SINGLE_CHAR_SIZE,
 unsigned char , sensor_current_enabled, MAX_SINGLE_CHAR_SIZE, // bool
 unsigned char , sensor_temperature_enabled, MAX_SINGLE_CHAR_SIZE, // bool
 unsigned char , sensor_vibration_enabled, MAX_SINGLE_CHAR_SIZE, // bool
 float , sensor_current_threshold_normal, MAX_SINGLE_CHAR_SIZE, 
 float , sensor_current_threshold_alert, MAX_SINGLE_CHAR_SIZE, 
 float , sensor_current_threshold_warning, MAX_SINGLE_CHAR_SIZE, 
 float , sensor_current_threshold_critical, MAX_SINGLE_CHAR_SIZE, 
 float , sensor_vibration_threshold_normal, MAX_SINGLE_CHAR_SIZE, 
 float , sensor_vibration_threshold_alert, MAX_SINGLE_CHAR_SIZE, 
 float , sensor_vibration_threshold_warning, MAX_SINGLE_CHAR_SIZE, 
 float , sensor_vibration_threshold_critical, MAX_SINGLE_CHAR_SIZE, 
 float , sensor_temperature_threshold_normal, MAX_SINGLE_CHAR_SIZE, 
 float , sensor_temperature_threshold_alert, MAX_SINGLE_CHAR_SIZE, 
 float , sensor_temperature_threshold_warning, MAX_SINGLE_CHAR_SIZE, 
 float , sensor_temperature_threshold_critical, MAX_SINGLE_CHAR_SIZE, 
 long  , sensor_current_sample_time_duration, MAX_SINGLE_LONG_SIZE, 
 long  , sensor_vibration_sample_time_duration, MAX_SINGLE_LONG_SIZE, 
 long  , sensor_temperature_sample_time_duration, MAX_SINGLE_LONG_SIZE, 
 long  , sensor_current_total_time_duration, MAX_SINGLE_LONG_SIZE, 
 long  , sensor_vibration_total_time_duration, MAX_SINGLE_LONG_SIZE, 
 long  , sensor_temperature_total_time_duration, MAX_SINGLE_LONG_SIZE, 
 long  , sensor_current_buzzer_light_notify_level, MAX_SINGLE_LONG_SIZE, 
 long  , sensor_temperature_buzzer_light_notify_level, MAX_SINGLE_LONG_SIZE, 
 long  , sensor_vibration_buzzer_light_notify_level, MAX_SINGLE_LONG_SIZE, 
 long  , development_print_level, MAX_SINGLE_LONG_SIZE,
 char , err_msg,  MAX_PATH_SIZE);

   

// data structure for the device configuraiton
// should be aligned with ENUM_MACRO
// typedef struct Device_config
// {
//  int config_id;
//  unsigned char whether_update_available;
//  char device_code_to_update_to[32]; // v000.000.000-000-gf676f80
//  char device_code_type[8];
//  char server_host_address_data[128]; // e.g. device1-eews.000webhostapp.com
//  int server_host_port_data;
//  char host_server_query_path[128];
//  char server_host_address_config[128]; // e.g. device1-eews.000webhostapp.com
//  int server_host_port_config;
//  char host_config_server_query_path[128];
//  char device_min_code_version[32]; // 000.000.000-000 or full v000.000.000-000-gf676f80
//  char device_max_code_version[32]; // 000.000.000-000 or full v000.000.000-000-gf676f80
//  int available_sensor_count;
//  unsigned char sensor_current_enabled; // bool
//  unsigned char sensor_temperature_enabled; // bool
//  unsigned char sensor_vibration_enabled; // bool
//  float sensor_current_threshold_normal;
//  float sensor_current_threshold_alert;
//  float sensor_current_threshold_warning;
//  float sensor_current_threshold_critical;
//  float sensor_vibration_threshold_normal;
//  float sensor_vibration_threshold_alert;
//  float sensor_vibration_threshold_warning;
//  float sensor_vibration_threshold_critical;
//  float sensor_temperature_threshold_normal;
//  float sensor_temperature_threshold_alert;
//  float sensor_temperature_threshold_warning;
//  float sensor_temperature_threshold_critical;
//  long sensor_current_sample_time_duration;
//  long sensor_vibration_sample_time_duration;
//  long sensor_temperature_sample_time_duration;
//  long sensor_current_total_time_duration;
//  long sensor_vibration_total_time_duration;
//  long sensor_temperature_total_time_duration;
//  long sensor_current_buzzer_light_notify_level;
//  long sensor_temerature_buzzer_light_notify_level;
//  long sensor_vibration_buzzer_light_notify_level;
//  long development_print_level; // bit-field
// }Device_config;

#define ENUM_MACRO_SHORT_DEVICE_INFO(name, offset \
,  tv1, v1, asv1 \
,  tv2, v2, asv2 \
,  tv3, v3, asv3 \
,  tv4, v4, asv4 \
,  tv5, v5, asv5 \
,  tv6, v6, asv6 \
,  tv7, v7, asv7 \
,  tv8, v8, asv8) \
\
    enum name##_enum { v1 =  offset, v2, v3, v4, v5, v6, v7\
    , v8}; \
    \
    static const char *name##_Strings[] = { #v1, #v2, #v3, #v4, #v5, #v6, #v7\
    , #v8};\
    \
    inline const char *name##_ToString(int value) { return name##_Strings[value - offset ]; }; \
 \
typedef struct  { \
tv1 v1[asv1]; \
tv2 v2[asv2]; \
tv3 v3[asv3]; \
tv4 v4[asv4]; \
tv5 v5[asv5]; \
tv6 v6[asv6]; \
tv7 v7[asv7]; \
tv8 v8[asv8]; \
    } name ; 

/* `config_id` 
,`whether_update_available` 
,`device_code_to_update_to` 
,`server_host_address_config` 
,`server_host_port_config` 
,`host_config_server_query_path`
 FROM `device_config`
*/

// ENUM_MACRO_SHORT_DEVICE_INFO(Device_update_info, 0, 
//     char, device_code_to_update_to, MAX_VER_STR_SIZE, 
//     int, config_id, MAX_SINGLE_CHAR_SIZE, 
//     unsigned char, whether_update_available, MAX_SINGLE_CHAR_SIZE, 
//     char, device_code_version, MAX_VER_STR_SIZE, 
//     char, server_host_address_config, MAX_PATH_SIZE, 
//     int, server_host_port_config, MAX_SINGLE_CHAR_SIZE, 
//     char, host_config_server_query_path, MAX_PATH_SIZE, 
//     char, query_path_with_versioned_file, (MAX_PATH_SIZE+MAX_VER_STR_SIZE));

// typedef struct Device_update_info
// {
//     char device_code_to_update_to[32]; // or  root["updatable_code_version"];
//     int config_id;
//     unsigned char whether_update_available;
//     char device_code_version[32];
//     char host_server_address[128];
//     int host_server_port;
//     char query_path[128];
//     char query_path_with_versioned_file[256];
// }Device_update_info;

// bool whether_post_wifi_connect_setup_done;


// 
// For HTTP web update following three variables should be used for the selection of upload
// _VER_, DEVICE_DEVELOPMENT_TYPE, DEVICE_ID
// The device ID becomes optional in case of _VER_ and DEVICE_DEVELOPMENT_TYPE

#define WIFISSID "HHA2204HH" // Put your WiFi SSID here
#define PASSWORD "hha2204hh" // Put your Wifi Password hereconst char *GScriptId = “—Your-Google-Script-ID–=”;


#define DISABLED (0)
#define ENABLED (1)

//#define TEMPERATURE_SUB_DEVICE (ENABLE)
#define CURRENT_SUB_DEVICE (DISABLED)
#define VIBRATION_SUB_DEVICE (ENABLED)

// const char *apiWritekey = "U6208ODGB0Q9SKJG";
// const char *ssid = WIFISSID;     //"JioFi3_3FA858";
// const char *password = PASSWORD; //"mnajk1h6tz" ;

// const char *server = "api.thingspeak.com";
// float resolution = 3.3 / 1023;

// GET https://api.thingspeak.com /update?api_key=K3CC6GPXNVQULRRX&field1=0&field1=0&field1=0
const int updateThingSpeakInterval = 10000; //* 1000; // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)

/// Local php server

#define php_server ("device1-eews.000webhostapp.com" )//"192.168.43.227")
#define php_server_port (80) //8000)
#define php_server_file_target ("/devices/insert_new_data.php?")
//const char *php_server_response ("New record created successfully.")

#define php_config_server ("device1-eews.000webhostapp.com" )//"192.168.43.227")
#define php_config_server_port (80) //8000)
#define php_config_server_file_target ("/devices/get_config.php?")
//const char *php_server_response ("New record created successfully.")

#define php_upgrade_server ("device1-eews.000webhostapp.com" )//"192.168.43.227")
#define php_upgrade_server_port (80) //8000)
#define php_upgrade_server_file_target (STRCAT(UPDATE_PATH, DEVICE_DEVELOPMENT_TYPE))
#define php_upgrade_server_status ("/devices/updated.php?")

 
// //#define TEST_MODE

// #define TELEGRAM_HOST "api.telegram.org"
// #define testBOTtoken "392509569:AAHNVA79fG9qsVQ9dgSS2GMXrZ5SLejEs_s"
// #define testBOTname "testBot"
// #define testBOTusername "test_watermotorstatus_bot"

// #define BOTtoken "444154317:AAFuKAx319tadCnWxHv9hdA0MiDbPLHxoj8" //token of TestBOT
// //#define BOTtoken (testBOTtoken)

// #define BOTname "Water-Electric-Motor"
// #define BOTusername "WaterElectricMotorBot"
// #define telegramDebugID "55129840"
// #define telegramGroupID "-237644374"

// // comment if GSCIPT BASED CONNECTION IS NEEDED
// #define PHP_HOST_BASED_CONNECTION

// ////////////////////////////////////////

// // https://in.000webhost.com/cpanel-login?from=panel
// // ID rahuldeo2047@gmail.com
// // PSSS whattokeep
// // index.php https://tricentennial-accum.000webhostapp.com/
// // logger.php https://tricentennial-accum.000webhostapp.com/logger.php?type=powerOn&ellapsed_time=33575&current_factor=0.13&motor_status=OFF&version=v0.0.1-2-gef42b98

// #define PHP_HOST "tricentennial-accum.000webhostapp.com" //"watermotor-datalogger.rf.gd" //logger.php?"

// #define PHP_HOST_1 "watermotordatalogger.000webhostapp.com"

// ////////////////////////////////////////
// #define GSCRIPT_DEPLOY_TYPE ("/dev?") //("/exec?")
// //https://script.google.com/macros/s/AKfycbz1paSaXa-JnXOJGBJSVCFCQmS0cw7XPtJnfbi6tnWsWlSrxRg/exec
// #define GScript_Id "AKfycbz1paSaXa-JnXOJGBJSVCFCQmS0cw7XPtJnfbi6tnWsWlSrxRg"
// //"AKfycbwmo0l2X7X2TcM9o1sSgSbULZ-Atbh67us1c4ePOehmjJkcvpeQ"
// //https://script.google.com/macros/s/AKfycbwmo0l2X7X2TcM9o1sSgSbULZ-Atbh67us1c4ePOehmjJkcvpeQ/exec
// //https://script.google.com/macros/s/AKfycbwmo0l2X7X2TcM9o1sSgSbULZ-Atbh67us1c4ePOehmjJkcvpeQ/exec

// #define GScript_host "script.google.com"
// #define GScript_googleRedirHost "script.googleusercontent.com"
// #define GScript_httpsPort (443)

// #define MOVING_AVERAGE_COUNT (10);

#define current_factor_threshold_for_motor_ON (1.0)

#define current_factor_threshold_for_motor_ON_No_Load (4.8)

// #define DISCONNECTION_TIME (60000)

//#define WIFISSID "D_ATOM_1_2"// Put your WiFi SSID here
//#define PASSWORD "D_ATOM_1" // Put your Wifi Password here

#endif // CONFIG_H

#ifndef COMMON_DEF_H
#define COMMON_DEF_H

#include "config_json_stream_parser.h"


void wifimanager_setup();

void syslog_debug(char* msg);
void syslog_info(char* msg);
void syslog_warn(char* msg);
void syslog_error(char* msg);

bool mpu_setup();
bool mpu_scan();
void mpu_loop();
float mpu_getTempFiltered();
float mpu_getTemp();
float mpu_getAccelFftMagFiltered();
float mpu_getAccelTwiceFftMagFiltered();
float mpu_getAccelFftMag();
void mpu_resetSampleTimer();

void Irms_setup();
void Irms_resetSampleTimer();
bool Irms_loop(); // It is not measuring status
float Irms_getCurr();
float Irms_getFilteredCurr();

bool setup_php_server();

void setup_OTA();

bool loop_OTA();

void mpu_loop();

//bool loop_php_server(unsigned long _php_sr, unsigned long _php_uptm, float _php_temp_f, float _php_temp_r, float _php_current_f, float _php_current_r, float _php_accel_f, float _php_accel_r);

ConfigListener * getJsonConfigListenerPtr();

void updateCodeUpdateStatus();
bool processConfig();

void setDeviceMacStr();
char * getDeviceMacStr();

void sendGraphDate(char* _device_id, char *message);

bool notifier_ledNotifierSetup();
void notifier_setNotifierState(NOTIFIER_STATES _state);
void notifier_ledNotifierLoop();


void setup_server_connection();
void loop_server_connection(unsigned long _php_sr, unsigned long _php_uptm, float _php_temp_f, float _php_temp_r, float _php_current_f, float _php_current_r, float _php_accel_f, float _php_accel_r);
void loop_config_server_connection();
bool server_is_data_available();
bool server_check_for_data();
bool server_parse_data();


#endif //COMMON_DEF_H
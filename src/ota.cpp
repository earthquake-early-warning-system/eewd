
//#include "data.h"

//GCC_DIAG_OFF(unused-parameter)
//GCC_DIAG_OFF(missing-field-initializers)

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "common_def.h"


//GCC_DIAG_ON(unused-parameter)
//GCC_DIAG_ON(missing-field-initializers)

#include "config.h"
// MAX_PRINT_BUFFER_SIZE
//char print_buffer[];// = {0}; //externed from config.h


boolean isSetupOK = false;
boolean isInProgramMode = false;
void setup_OTA()
{
  //Serial.begin(115200);
  //Serial.print("OTA config ... ");

  if (!WiFi.isConnected())
  {
    //Serial.printf("ERROR: Wifi not connected. Please connect to the wifi first\n");
    isSetupOK = false;
    return;
  }

  // Assumint it is called if system is connected to the WiFi
  //
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);
  // while (WiFi.waitForConnectResult() != WL_CONNECTED) {
  //   Serial.println("Connection Failed! Rebooting...");
  //   delay(5000);
  //   ESP.restart();
  // }

  // Port defaults to 8266
  if (isSetupOK)
    return; // already cofigured

  ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    isInProgramMode = true;
    // if (ArduinoOTA.getCommand() == U_FLASH)
    //   type = "sketch";
    // else // U_SPIFFS
    //   type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    //Serial.println("Start updating ?" + type);
    sprintf(getPrintBuffer(), "Start local OTA update ... ?");
    //Serial.println();
    Serial.println(getPrintBuffer());
    syslog_debug(getPrintBuffer());
  });
  ArduinoOTA.onEnd([]() {
    isInProgramMode = false;
    //Serial.println("\nEnd");
    sprintf(getPrintBuffer(), "Ended local OTA update.");
    //Serial.println();
    Serial.println(getPrintBuffer());
    syslog_debug(getPrintBuffer());
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    isInProgramMode = true;
    sprintf(getPrintBuffer(), "Progress local OTA: %u%%", (progress / (total / 100)));
    //Serial.println();
    Serial.println(getPrintBuffer());
    syslog_debug(getPrintBuffer());
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    isInProgramMode = false;
    sprintf(getPrintBuffer(), "Error[%u]: ", error);
    //Serial.println();
    Serial.println(getPrintBuffer());
    syslog_debug(getPrintBuffer());
    //Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      sprintf(getPrintBuffer(), "Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      sprintf(getPrintBuffer(), "Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      sprintf(getPrintBuffer(), "Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      sprintf(getPrintBuffer(), "Receive Failed");
    else if (error == OTA_END_ERROR)
      sprintf(getPrintBuffer(), "End Failed");

    Serial.println(getPrintBuffer());
    syslog_debug(getPrintBuffer());
  });
  ArduinoOTA.begin();
  isSetupOK = true;
  sprintf(getPrintBuffer(), "Ready | IP address: %s | In ubutnu please run following command to configure firewall => $ sudo ufw allow from %s", WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());
  //Serial.println();
  Serial.println(getPrintBuffer());
  syslog_debug(getPrintBuffer());

  // Serial.println("Ready");
  // Serial.print("IP address: ");Serial.println(WiFi.localIP());
  // Serial.print("In ubutnu please run following command to configure firewall\n\t\tsudo ufw allow from ");
  // Serial.println(WiFi.localIP());
}

bool loop_OTA()
{
  if (!isSetupOK)
    return false;

  ArduinoOTA.handle();

  return isInProgramMode;
}

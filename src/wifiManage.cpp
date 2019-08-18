// Following shifted to main
//#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#include "common_def.h"

void wifimanager_setup() {
  notifier_setNotifierState(NOTIFIER_STATES::_1_LED_WIFI_OFFLINE_MODE);

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //exit after config instead of connecting
  //wifiManager.setBreakAfterConfig(true);

  //reset settings - for testing
  //wifiManager.resetSettings();


  //tries to connect to last known settings
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP" with password "password"
  //and goes into a blocking loop awaiting configuration
  String pass = String(WiFi.macAddress());
  pass.replace(":", "");

  String ssid = "EEWD_" + pass;

  notifier_setNotifierState(NOTIFIER_STATES::_1_LED_WIFI_CONFIG);
  
  if (!wifiManager.autoConnect(ssid.c_str(), pass.c_str())){//"AutoConnectAP", "password")) {
      notifier_setNotifierState(NOTIFIER_STATES::_1_LED_WIFI_CONN_FAILED);

    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  notifier_setNotifierState(NOTIFIER_STATES::_1_LED_WIFI_CONNECTED);

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");


  Serial.println("local ip");
  Serial.println(WiFi.localIP());
}
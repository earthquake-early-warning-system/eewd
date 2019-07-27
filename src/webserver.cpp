#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "index.h"

ESP8266WebServer _server(80); //Server on port 80

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================

float tmp_f, tmp_r, current_f, current_r, accel_f, accel_r;
unsigned long sr_ser, uptm;

void handleRoot()
{
  String s = MAIN_page;              //Read HTML contents
  _server.send(200, "text/html", s); //Send web page
}

void handleData()
{

  String sr_str = String(sr_ser);
  String uptm_str = String(uptm);

  String tempValue_f = String(tmp_f);
  String tempValue_r = String(tmp_r);

  String currValue_f = String(current_f);
  String currValue_r = String(current_r);

  String accValue_f = String(accel_f);
  String accValue_r = String(accel_r);

  String allData = sr_str + "," + uptm_str + "," + tempValue_f + "," + tempValue_r + "," + currValue_f + "," + currValue_r + "," + accValue_f + "," + accValue_r;

  _server.send(200, "text/plane", allData); //Send ADC value only to client ajax request
}

void setup_server()
{
  _server.on("/", handleRoot);         //Which routine to handle at root location. This is display page
  _server.on("/readData", handleData); //This page is called by java Script AJAX

  _server.begin(); //Start server
}

void loop_server(unsigned long _sr, unsigned long _uptm, float _temp_f, float _temp_r, float _current_f, float _current_r, float _accel_f, float _accel_r)
{
  sr_ser = _sr;
  uptm = _uptm;
  tmp_f = _temp_f;
  tmp_r = _temp_r;
  current_f = _current_f;
  current_r = _current_r;
  accel_f = _accel_f;
  accel_r = _accel_r;
}

void handleClients()
{
  _server.handleClient(); //Handle client requests
}
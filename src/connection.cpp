#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>

extern "C" {
#include <osapi.h>
#include <os_type.h>
}



#include <Arduino.h>
#include "config.h"

#include "JsonStreamingParser.h"
#include "JsonListener.h"
#include "config_json_stream_parser.h"
  

#include "config.h"

AsyncClient* client = new AsyncClient;


 

static void replyToServer(void* arg) {
	AsyncClient* client = reinterpret_cast<AsyncClient*>(arg);

	// send reply
	if (client->space() > 32 && client->canSend()) {
		char message[32];
		sprintf(message, "this is from %s", WiFi.localIP().toString().c_str());
		client->add(message, strlen(message));
		client->send();
	}
}

static void sendData(String _msg) 
{
	 
	// send data
	if (client->space() > _msg.length() && client->canSend()) 
    {
		client->add(_msg.c_str(), _msg.length());
		client->send();
	}
}

/* event callbacks */
static void handleData(void* arg, AsyncClient* client, void *data, size_t len) 
{
	Serial.printf("\n data received from %s \n", client->remoteIP().toString().c_str());
	Serial.write((uint8_t*)data, len);

	//os_timer_arm(&intervalTimer, 2000, true); // schedule for reply to server at next 2s
}

void onConnect(void* arg, AsyncClient* client) 
{
	Serial.printf("\n client has been connected to %s on port %d \n", php_server, php_server_port);
	//replyToServer(client);

    String mac_str = (WiFi.macAddress());
    mac_str.replace(":", "");
    
    String data_str = "device_code_type=" + String(DEVICE_DEVELOPMENT_TYPE) + "&config_id=" + String(config_id) + "&config_type=l" // long or short
                      + "&device_code_version=" + _VER_
                      + "&Device_mac_id_str="+mac_str;

    String getStr = "GET " + String(php_server_file_target) + data_str + " HTTP/1.1\r\nHost: " + String(php_server) + "\r\n\r\n";
 
    sendData(data_str);

} 

void onDisconnect(void* arg, AsyncClient* client) 
{
	Serial.printf("\n client has been disconnected from %s on port %d \n", php_server, php_server_port);
	//replyToServer(client);
}

void setup_server_connection()
{  
	client->onData(&handleData, client);
	client->onConnect(&onConnect, client);
    client->onDisconnect(&onDisconnect, client);
	client->connect(php_server, php_server_port);// php_server_file_target
}
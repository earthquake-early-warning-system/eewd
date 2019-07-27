// #if defined(ESP8266)

// #define USE_MDNS true // Use the MDNS ?

// // Includes do ESP8266

// #include <ESP8266WiFi.h>

// #ifdef USE_MDNS
// #include <DNSServer.h>
// #include <ESP8266mDNS.h>
// #endif

// #include "common_def.h"


// #elif defined(ESP32)


// // Incomplete

// //#define USE_MDNS true // Use the MDNS ? //TODO: not tested in Esp32 yet

// // Includes do ESP32

// #include <WiFi.h>

// #ifdef USE_MDNS
// #include "ESPmDNS.h"
// #endif

// #else

// #error The board must be ESP8266 or ESP32

// #endif // ESP

// // Remote debug over telnet - not recommended for production, only for development

// #include "RemoteDebug.h" //https://github.com/JoaoLopesF/RemoteDebug

// // Time

// uint32_t mLastTime = 0;
// uint32_t mTimeSeconds = 0;

// //#define HOST_NAME "remotedebug-heavy_wind_alert"

// void rd_setup(const char *HOST_NAME)
// {
// 	// Register host name in WiFi and mDNS

// 	String hostNameWifi = HOST_NAME;
// 	hostNameWifi.concat(".local");

// #ifdef ESP8266 // Only for it
// 	WiFi.hostname(hostNameWifi);
// #endif

// #ifdef USE_MDNS // Use the MDNS ?

// 	if (MDNS.begin(HOST_NAME))
// 	{
// 		Serial.print("* MDNS responder started. Hostname -> ");
// 		Serial.println(HOST_NAME);
// 	}

// 	MDNS.addService("telnet", "tcp", 23);

// #endif

// 	// Initialize the telnet server of RemoteDebug

// 	Debug.begin(HOST_NAME); // Initiaze the telnet server

// 	Debug.setResetCmdEnabled(true); // Enable the reset command

// 	//?

// 	//String helpCmd = "+ - increase distance\n";
// 	//helpCmd.concat("- - decrease distance");

// 	//Debug.setHelpProjectsCmds(helpCmd);
// 	//Debug.setCallBackProjectCmds(&processCmdRemoteDebug);
// }

// void rd_loop()
// {
// 	Debug.handle();
// 	//DEBUG_V("* Time: %u seconds (VERBOSE)\n", mTimeSeconds);
// 	yield();
// }

// void processCmdRemoteDebug()
// {

// 	String lastCmd = Debug.getLastCommand();

// 	if (lastCmd == "+")
// 	{

// 		// Benchmark 1 - Printf

// 		if (Debug.isActive(Debug.ANY))
// 		{
// 			Debug.println("* increase distance");
// 		}

// 		uint32_t timeBegin = millis();
// 		uint8_t times = 50;

// 		//DISTANCE_BETWEEN_TX_RX += 0.1;

// 		if (Debug.isActive(Debug.ANY))
// 		{
// 			Debug.printf("* Time elapsed for %u printf: %ld ms.\n", times,
// 						 (millis() - timeBegin));
// 		}
// 	}
// 	else if (lastCmd == "-")
// 	{

// 		// Benchmark 2 - Print/println

// 		if (Debug.isActive(Debug.ANY))
// 		{
// 			Debug.println("* decrease distance");
// 		}

// 		uint32_t timeBegin = millis();
// 		uint8_t times = 50;

// 		//DISTANCE_BETWEEN_TX_RX -= 0.1;

// 		if (Debug.isActive(Debug.ANY))
// 		{
// 			Debug.printf("* Time elapsed for %u printf: %ld ms.\n", times,
// 						 (millis() - timeBegin));
// 		}
// 	}
// }

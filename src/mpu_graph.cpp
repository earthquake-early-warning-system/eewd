#include <Arduino.h>
#include <WiFiUdp.h> 
#include "config.h"

#include "common_def.h"

#define MAX_PACKET_SIZE_BUFF 128
WiFiUDP udp;

#define GRAPH_SERVER_ADDRESS SYSLOG_SERVER_ADDRESS
#define GRAPH_SERVER_POTR 12321


void sendGraphDate(char* _device_id, char *message) 
{
  uint8_t buffer[MAX_PACKET_SIZE_BUFF];
  uint16_t len = snprintf((char *)buffer, MAX_PACKET_SIZE_BUFF, "%d, %s, %s\n", millis(), _device_id, message);
  len = (len > MAX_PACKET_SIZE_BUFF)?MAX_PACKET_SIZE_BUFF:len; // handle truncation if message too long
  udp.beginPacket(GRAPH_SERVER_ADDRESS, GRAPH_SERVER_POTR);
  udp.write(buffer, len);
  udp.endPacket();
}
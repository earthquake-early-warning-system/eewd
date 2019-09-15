#include <Arduino.h>

#include "config.h"
#include "common_def.h"


enum JUMPER_PERIPHERAL
{ 
 JUMPER_PERIPHERAL__OFFLINE_MODE=(D0) 
};


void jumper_setup()
{ 
	pinMode(JUMPER_PERIPHERAL__OFFLINE_MODE, INPUT); //INPUT_PULLUP
	digitalWrite(JUMPER_PERIPHERAL__OFFLINE_MODE, HIGH);
}

bool jumper_offline_mode_status()
{
    return digitalRead(JUMPER_PERIPHERAL__OFFLINE_MODE);
}
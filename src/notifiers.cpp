#include <Arduino.h>

#include "config.h"
#include "common_def.h"

#include <Ticker.h>
#include <jled.h>


#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT (TOSTRING(__LINE__))
void debug_print(const char *location, const char *msg)
{
  //Serial.printf(" %s: %s\n", location, msg);
}

//debug_print(AT, "fake error");
 
#define DEBUG_LOG(txt) debug_print(AT, txt);


enum LED_NOTIFIER_HEARTBEAT_STATE // Should be equal to LED_WHITE_BREATHE_STATE
{
    LED_HEARTBEAT_STATE__OFF, // LED OFF
	LED_HEARTBEAT_STATE__OFFLINE, // LED blink indicating offline mode
    LED_HEARTBEAT_STATE__NORMAL, 
    LED_HEARTBEAT_STATE__MID,
    LED_HEARTBEAT_STATE__FAST,
    LED_HEARTBEAT_STATE__VERY_FAST
};


enum LED_WHITE_BREATHE_STATE // Should be equal to LED_NOTIFIER_HEARTBEAT_STATE
{
  LED_WHITE_BREATHE_STATE__OFF, // LED OFF
  LED_WHITE_BREATHE_STATE__OFFLINE,
  LED_WHITE_BREATHE_STATE__NORMAL,
  LED_WHITE_BREATHE_STATE__MID,
  LED_WHITE_BREATHE_STATE__FAST,
  LED_WHITE_BREATHE_STATE__VERY_FAST,
  LED_WHITE_BREATHE_STATE__INVALID //= LED_WHITE_BREATHE_STATE__VERY_FAST
};

enum LED_NOTIFIER_CODE_BURN_STATE
{
    LED_NOTIFIER_CODE_BURN_STATE__OFFLINE, 
    LED_NOTIFIER_CODE_BURN_STATE__LOCAL,
    LED_NOTIFIER_CODE_BURN_STATE__REMOTE
};


enum LED_NOTIFIER_WIFI_STATE
{
    LED_NOTIFIER_WIFI_STATE__OFFLINE,
    LED_NOTIFIER_WIFI_STATE__CHECK,
    LED_NOTIFIER_WIFI_STATE__CONFIG,
	LED_NOTIFIER_WIFI_STATE__CONN_FAILED,
    LED_NOTIFIER_WIFI_STATE__CONNECTED 
};

enum LED_NOTIFIER_SERVER_STATE
{
    LED_NOTIFIER_SERVER_STATE__OFFLINE,
    LED_NOTIFIER_SERVER_STATE__CONNECTING,
    LED_NOTIFIER_SERVER_STATE__CONNECTED,
    LED_NOTIFIER_SERVER_STATE__CONN_FAILD,
    LED_NOTIFIER_SERVER_STATE__DATA_SENDING,
    LED_NOTIFIER_SERVER_STATE__DATA_SENT 
};

enum LED_NOTIFIER_SENSOR_STATE
{
    LED_NOTIFIER_SENSOR_STATE__OK,
    LED_NOTIFIER_SENSOR_STATE__NOTIFY,
    LED_NOTIFIER_SENSOR_STATE__ALERT,
    LED_NOTIFIER_SENSOR_STATE__WARN,
    LED_NOTIFIER_SENSOR_STATE__EMERGENCY,
	LED_NOTIFIER_SENSOR_STATE__CRITICAL 
};

#define LED_CONFIG_TYPE (2) // (2)
 
#if (LED_CONFIG_TYPE==1) 
enum LED_PERIPHERAL
{ 
 LED_PERIPHERAL_WHITE=(D8),
 LED_PERIPHERAL_BLUE_1=(D1),
 LED_PERIPHERAL_BLUE_2=(D2),
 LED_PERIPHERAL_YELLOW=(D3),
 LED_PERIPHERAL_RED=(D4) 
};
#elif (LED_CONFIG_TYPE==2) 
enum LED_PERIPHERAL
{ 
 LED_PERIPHERAL_WHITE=(D8),
 LED_PERIPHERAL_BLUE_1=(D3),
 LED_PERIPHERAL_BLUE_2=(D4),
 LED_PERIPHERAL_YELLOW=(D1),
 LED_PERIPHERAL_RED=(D2) 
};
#else
#error "invalid led config type"
#endif 

enum LED_ID
{
	LED_ID_WHITE=(0),
	LED_ID_BLUE_1=(1),
	LED_ID_BLUE_2=(2),
	LED_ID_YELLOW=(3),
	LED_ID_RED=(4) ,
	LED_ID_MAX=(LED_ID_RED+1) 
};


LED_WHITE_BREATHE_STATE led_white_breahe_state;

#define LED_WHITE_BREATHE_STATE_PLUS_ONE (1)
JLed led_breathe_white[LED_WHITE_BREATHE_STATE__INVALID] =
{
		JLed(LED_PERIPHERAL_WHITE).Stop(), // Dummy
		JLed(LED_PERIPHERAL_WHITE).Blink(50, 9950).Forever(), // LED_WHITE_BREATHE_STATE__OFFLINE
		JLed(LED_PERIPHERAL_WHITE).Blink(10, 4950).Forever(), // LED_WHITE_BREATHE_STATE__NORMAL
		JLed(LED_PERIPHERAL_WHITE).Blink(5, 1950).Forever(),  //LED_WHITE_BREATHE_STATE__MID
		JLed(LED_PERIPHERAL_WHITE).Blink(5, 150).Forever(),   //LED_WHITE_BREATHE_STATE__FAST
		JLed(LED_PERIPHERAL_WHITE).Blink(5, 50).Forever()	 //LED_WHITE_BREATHE_STATE__VERY_FAST
};

Ticker flipper, flipper_white, flipper_blue_1, flipper_blue_2, flipper_yellow, flipper_red;

struct LED_STATE
{
	uint8_t LED_PERI;
	bool LED_state;
	Ticker flipper;
};

// This does not work
// struct LED_STATE led_states[]=
// {
//   {flipper_white, LED_WHITE, false},
//   {flipper_blue_1, LED_BLUE_1, false},
//   {flipper_blue_2, LED_BLUE_2, false},
//   {flipper_yellow, LED_YELLOW, false},
//   {flipper_red, LED_RED, false}
// };

// struct padding is a big issue
struct LED_STATE led_states[LED_ID_MAX] =
{
		{LED_PERIPHERAL_WHITE, false, flipper_white},
		{LED_PERIPHERAL_BLUE_1, false, flipper_blue_1},
		{LED_PERIPHERAL_BLUE_2, false, flipper_blue_2},
		{LED_PERIPHERAL_YELLOW, false, flipper_yellow},
		{LED_PERIPHERAL_RED, false, flipper_red}
};

int count = 0;

void flip_d(const uint8_t led_id)
{
	int pin_state = digitalRead(led_states[led_id].LED_PERI); // get the current state of GPIO1 pin
															  //bool state = led_states[led_id].LED_state;
	led_states[led_id].LED_state = pin_state;

	digitalWrite(led_states[led_id].LED_PERI, !pin_state); // set pin to the opposite state
}

// led_id and freq if last_state==true it sets the state as active low 'ON' 
void ledState(LED_ID led_id, float freq, bool last_state=false)
{
	float ms = 0;
	if (freq == 0)
	{
		led_states[led_id].flipper.detach();
		if(last_state)
		{
			digitalWrite(led_states[led_id].LED_PERI, 0); // active low 'ON'
		}
	}
	else
	{
		ms = (1000.0f / freq);
		led_states[led_id].flipper.attach_ms_scheduled(ms, std::bind(flip_d, led_id));
	}
}

bool has_notifier_setup=false;
bool notifier_ledNotifierSetup()
{
	// pinMode(LED_BUILTIN, OUTPUT);
	// digitalWrite(LED_BUILTIN, LOW);

	//pinMode(D4 /* D9*/, OUTPUT);
	//digitalWrite(D4 /* D9*/, HIGH);

	pinMode(LED_PERIPHERAL_WHITE, OUTPUT);
	digitalWrite(LED_PERIPHERAL_WHITE, HIGH);

	pinMode(LED_PERIPHERAL_BLUE_1, OUTPUT);
	digitalWrite(LED_PERIPHERAL_BLUE_1, HIGH);

	 pinMode(LED_PERIPHERAL_BLUE_2, OUTPUT);
	 digitalWrite(LED_PERIPHERAL_BLUE_2, HIGH);

	pinMode(LED_PERIPHERAL_YELLOW, OUTPUT);
	digitalWrite(LED_PERIPHERAL_YELLOW, HIGH);

	pinMode(LED_PERIPHERAL_RED, OUTPUT);
	digitalWrite(LED_PERIPHERAL_RED, HIGH);

	// flip the pin every 0.3s
	//flipper.attach(0.3, flip);

	led_white_breahe_state = LED_WHITE_BREATHE_STATE__OFFLINE;
	//ledState(LED_ID_WHITE, 1.0);

	//Serial.println("notify setup");
	has_notifier_setup = true;
	sprintf_P(getPrintBuffer(), "status_notify :%d\n", has_notifier_setup);
    DEBUG_LOG(getPrintBuffer());

	

	return true;
}

void notifier_ledNotifierLoop()
{

	if(false == has_notifier_setup)
	{
		return;
	}	

	if (led_white_breahe_state != LED_WHITE_BREATHE_STATE__INVALID)
	{
		if(
			(led_white_breahe_state == LED_HEARTBEAT_STATE__OFF)
			||
			(led_white_breahe_state == LED_WHITE_BREATHE_STATE__OFF)
		)
		{
			led_breathe_white[led_white_breahe_state].Stop();
		}
		else
		{
			
			led_breathe_white[led_white_breahe_state].Update();

		}
		
	}
}

void setLedNotifierHBState(LED_NOTIFIER_HEARTBEAT_STATE _led_bh_state)
{ 
    // Both the enums must be of same size
	if(LED_WHITE_BREATHE_STATE__INVALID > (LED_WHITE_BREATHE_STATE)_led_bh_state)
	{ 
		ledState(LED_ID_WHITE, 0.0);  
		led_white_breahe_state = (LED_WHITE_BREATHE_STATE)_led_bh_state;
		sprintf_P(getPrintBuffer(), "hb notify state :%d\n", led_white_breahe_state);
    	DEBUG_LOG(getPrintBuffer());
	}
	else
	{
		sprintf_P(getPrintBuffer(), "Notifier: LED_WHITE_BREATHE_STATE__INVALID at %d\n", __LINE__);
		Serial.printf(getPrintBuffer());
		//syslog_warn(getPrintBuffer());
	} 
}

void setLedNotofierCodeBurnState(LED_NOTIFIER_CODE_BURN_STATE _state)
{

	sprintf_P(getPrintBuffer(), "burn notify state :%d\n", _state);
    DEBUG_LOG(getPrintBuffer());

	switch (_state)
	{
		case LED_NOTIFIER_CODE_BURN_STATE__OFFLINE :
			//ledState(LED_ID_WHITE, 0); // No need to change earlier white led state
			break;

			//_0_NOTIFIER_HB_PING
		
		case LED_NOTIFIER_CODE_BURN_STATE__LOCAL :
			ledState(LED_ID_WHITE, 2.0);  
			//setLedNotifierHBState(LED_HEARTBEAT_STATE__OFF);
			break;

		case LED_NOTIFIER_CODE_BURN_STATE__REMOTE :
			ledState(LED_ID_WHITE, 4.0);
			//setLedNotifierHBState(LED_HEARTBEAT_STATE__OFF);
			break;			
		
		//  no further state required as the device will reset
		
		default:
			break;
	}
	
}

void setLedNotifierWIFIState(LED_NOTIFIER_WIFI_STATE _state)
{  
	sprintf_P(getPrintBuffer(), "wifi notify state :%d\n", _state);
    DEBUG_LOG(getPrintBuffer());

	switch (_state)
	{
		case LED_NOTIFIER_WIFI_STATE__OFFLINE :
		    ledState(LED_ID_BLUE_1, 0, true); 
			digitalWrite(led_states[LED_ID_BLUE_1].LED_PERI, HIGH);// active low 'OFF'
			//ledState(LED_ID_BLUE_1, 0); // No need to change earlier white led state
			break;
		
		case LED_NOTIFIER_WIFI_STATE__CHECK :
			ledState(LED_ID_BLUE_1, 2.0);  
			break;

		case LED_NOTIFIER_WIFI_STATE__CONN_FAILED :
			ledState(LED_ID_BLUE_1, 8.0);  
			break;

		case LED_NOTIFIER_WIFI_STATE__CONFIG :
			ledState(LED_ID_BLUE_1, 4.0);
			break;			

		case LED_NOTIFIER_WIFI_STATE__CONNECTED :
			ledState(LED_ID_BLUE_1, 0); 
			digitalWrite(led_states[LED_ID_BLUE_1].LED_PERI, LOW);// active low 'OFF'
			break;
		
		default:
			break;
	}
}

void setLedNotifierServerState(LED_NOTIFIER_SERVER_STATE _state)
{  

	sprintf_P(getPrintBuffer(), "server notify state :%d\n", _state);
    DEBUG_LOG(getPrintBuffer());

	switch (_state)
	{
		case LED_NOTIFIER_SERVER_STATE__OFFLINE :
			//ledState(LED_ID_BLUE_2, 0); // No need to change earlier white led state
			break;
		
		case LED_NOTIFIER_SERVER_STATE__CONNECTING :
			ledState(LED_ID_BLUE_2, 2.0);  
			break;

		case LED_NOTIFIER_SERVER_STATE__CONNECTED :
			ledState(LED_ID_BLUE_2, 4.0);
			break;			

		case LED_NOTIFIER_SERVER_STATE__CONN_FAILD :
			ledState(LED_ID_BLUE_2, 8.0); 
			break;

		case LED_NOTIFIER_SERVER_STATE__DATA_SENDING :
			ledState(LED_ID_BLUE_2, 4.0);
			break;			

		case LED_NOTIFIER_SERVER_STATE__DATA_SENT :
			ledState(LED_ID_BLUE_2, 0); 
			digitalWrite(led_states[LED_ID_BLUE_2].LED_PERI, LOW);// active low 'OFF'


			break;
				
		default:
			break;
	}
	
}

void setLedNotifierSensorState(LED_NOTIFIER_SENSOR_STATE _state)
{  
	sprintf_P(getPrintBuffer(), "sense notify state :%d\n", _state);
    DEBUG_LOG(getPrintBuffer());

	switch (_state)
	{
		case LED_NOTIFIER_SENSOR_STATE__OK :
			ledState(LED_ID_YELLOW, 0); //  need to change earlier yellow led state
			ledState(LED_ID_RED, 0); // need to change earlier red led state

			digitalWrite(led_states[LED_ID_YELLOW].LED_PERI, HIGH);// active low 'OFF'
			digitalWrite(led_states[LED_ID_RED].LED_PERI, HIGH);// active low 'OFF'

			break;
		
		case LED_NOTIFIER_SENSOR_STATE__NOTIFY :
			ledState(LED_ID_YELLOW, 2.0);  
			ledState(LED_ID_RED, 0);
			digitalWrite(led_states[LED_ID_RED].LED_PERI, HIGH);// active low 'OFF'

			break;

		case LED_NOTIFIER_SENSOR_STATE__ALERT :
			ledState(LED_ID_YELLOW, 4.0);
			ledState(LED_ID_RED, 0);
			digitalWrite(led_states[LED_ID_RED].LED_PERI, HIGH);// active low 'OFF'

			break;			

		case LED_NOTIFIER_SENSOR_STATE__WARN :
			ledState(LED_ID_YELLOW, 8.0); 
			ledState(LED_ID_RED, 2.0); 
			break;

		case LED_NOTIFIER_SENSOR_STATE__EMERGENCY :
			ledState(LED_ID_YELLOW, 8.0); 
			ledState(LED_ID_RED, 4.0); 
			break;			 

		case LED_NOTIFIER_SENSOR_STATE__CRITICAL :

			// Continuous on
					
			ledState(LED_ID_YELLOW, 0); //  need to change earlier yellow led state
			ledState(LED_ID_RED, 0); // need to change earlier red led state

			digitalWrite(led_states[LED_ID_YELLOW].LED_PERI, LOW);// active low 'OFF'
			digitalWrite(led_states[LED_ID_RED].LED_PERI, LOW);// active low 'OFF'

			
			break;			 

		default:
			break;
	}
	
}


void notifier_setNotifierState(NOTIFIER_STATES _state)
{

	if(false == has_notifier_setup)
	{
		sprintf_P(getPrintBuffer(), "warn | notify state :has_notifier_setup: %d\n", has_notifier_setup);
    	DEBUG_LOG(getPrintBuffer());
		return;
	}	

	sprintf_P(getPrintBuffer(), "notify state :%d\n", _state);
    DEBUG_LOG(getPrintBuffer());


	// if(_0_NOTIFIER_HB_OFFLINE_MODE!=_state)
	// return;

	switch (_state)
	{
	case _0_NOTIFIER_HB_OFFLINE_MODE :
		setLedNotifierHBState(LED_HEARTBEAT_STATE__OFFLINE);
		break;

    case _0_NOTIFIER_HB_PING :
		setLedNotifierHBState(LED_HEARTBEAT_STATE__NORMAL);
		break;

    case _0_NOTIFIER_CODE_ERROR :
		ledState(LED_ID_WHITE, 10.0);
		//setLedNotifierHBState(LED_HEARTBEAT_STATE__NORMAL);
		break;
		 
	case _0_NOTIFIER_LOCAL_CODE_BURN_STARTED :
		setLedNotofierCodeBurnState(LED_NOTIFIER_CODE_BURN_STATE__LOCAL);
		break;
	case _0_NOTIFIER_REMOTE_CODE_BURN_STARTED :
		setLedNotofierCodeBurnState(LED_NOTIFIER_CODE_BURN_STATE__REMOTE);
		break;
	case _1_LED_WIFI_CHECK :
		setLedNotifierWIFIState(LED_NOTIFIER_WIFI_STATE__CHECK);
		break;

	case _1_LED_WIFI_CONFIG :
		setLedNotifierWIFIState(LED_NOTIFIER_WIFI_STATE__CONFIG);
		break;
	
	case _1_LED_WIFI_CONNECTED :
		setLedNotifierWIFIState(LED_NOTIFIER_WIFI_STATE__CONNECTED);
		break;
	
	case _1_LED_WIFI_CONN_FAILED:
		setLedNotifierWIFIState(LED_NOTIFIER_WIFI_STATE__CONN_FAILED);
		break;

	case _1_LED_WIFI_OFFLINE_MODE :
		setLedNotifierWIFIState(LED_NOTIFIER_WIFI_STATE__OFFLINE);
		break;
	
	case _2_LED_SERVER_CONNECTING :
		setLedNotifierServerState(LED_NOTIFIER_SERVER_STATE__CONNECTING); 
		break;

	case _2_LED_SERVER_CONNECTED :
		setLedNotifierServerState(LED_NOTIFIER_SERVER_STATE__CONNECTED); 
		break;

	case _2_LED_SERVER_CONN_FAILED :
		setLedNotifierServerState(LED_NOTIFIER_SERVER_STATE__CONN_FAILD); 
		break;

	case _2_LED_SERVER_DATA_SENDING :
		setLedNotifierServerState(LED_NOTIFIER_SERVER_STATE__DATA_SENDING); 
		break;

	case _2_LED_SERVER_DATA_SENT :
		setLedNotifierServerState(LED_NOTIFIER_SERVER_STATE__DATA_SENT); 
		break;
	
	case _3_LED_SENSOR_OK:
		setLedNotifierSensorState(LED_NOTIFIER_SENSOR_STATE__OK); 
		break;
	
	case _3_LED_SENSOR_NOTIFY:
		setLedNotifierSensorState(LED_NOTIFIER_SENSOR_STATE__NOTIFY); 
		break;
	
	case _3_LED_SENSOR_ALERT:
		setLedNotifierSensorState(LED_NOTIFIER_SENSOR_STATE__ALERT); 
		break;
	
	case _4_LED_SENSOR_WARN:
		setLedNotifierSensorState(LED_NOTIFIER_SENSOR_STATE__WARN); 
		break;
	
	case _4_LED_SENSOR_EMERGENCY:
		setLedNotifierSensorState(LED_NOTIFIER_SENSOR_STATE__EMERGENCY); 
		break; 

	case _4_LED_SENSOR_CRITICAL:
		setLedNotifierSensorState(LED_NOTIFIER_SENSOR_STATE__CRITICAL); 
		break; 

	default:
		break;
	}
}
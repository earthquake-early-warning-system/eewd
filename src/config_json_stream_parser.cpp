
#include "config_json_stream_parser.h"
#include "JsonListener.h"

#include "common_def.h"

#include "config.h"

//char print_buffer[256] = {0}; //externed from config.h

void ConfigListener::print(char *msg)
{
  Serial.println(msg);
  syslog_debug(msg);
}

Device_config * ConfigListener::getDeviceConfigPtr()
{
  return &device_config_;
}

void ConfigListener::whitespace(char c)
{
  //sprintf(getPrintBuffer(), "whitespace %c", c);
  //this->print(getPrintBuffer());
}

void ConfigListener::startDocument()
{
  //sprintf(getPrintBuffer(), "start document");
  //this->print(getPrintBuffer());
}

void ConfigListener::key(String key)
{
  this->current_key_ = key;
  //sprintf(getPrintBuffer(), "key: %s", key.c_str());
  //this->print(getPrintBuffer());
}

void ConfigListener::value(String value)
{
  int i, index = -1;

  for (i = 0; i < Device_config_enum::vSize; i++)
  {
    String check_key = Device_config_ToString(i);
    if (this->current_key_ == check_key)
    {
      index = i;
      break;
    }
  }

  if (index == -1)
  {
    sprintf(getPrintBuffer(), "cjsp: %d | No valid key received.", __LINE__);
    this->print(getPrintBuffer());
    return;
  }

  sprintf(getPrintBuffer(), " key: %s value: %s", Device_config_ToString(index), value.c_str());
  this->print(getPrintBuffer());

  switch (index)
  {
  case Device_config_enum::config_id:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.config_id[0] = value.toInt();
  }
  break;

  case Device_config_enum::device_id:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.device_id[0] = value.toInt();
  }
  break;

  case Device_config_enum::whether_update_available:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.whether_update_available[0] = value.toInt();
  }
  break;

  // v000.000.000-000-gf676f80
  case Device_config_enum::device_code_to_update_to:
  {
    // MAX_VER_STR_SIZE
    unsigned int valid_size =  value.length() > MAX_VER_STR_SIZE ? MAX_VER_STR_SIZE : value.length();
    strncpy(device_config_.device_code_to_update_to, value.c_str(), valid_size);
    //value.toCharArray(
    //    device_config_.device_code_to_update_to, valid_size);

    if (value.length() > MAX_VER_STR_SIZE)
    {
      sprintf(getPrintBuffer(), "cjsp: %d | %s | Possible data truncation.", __LINE__, Device_config_ToString(index));
      this->print(getPrintBuffer());
    }
  }
  break;
  case Device_config_enum::device_code_type:
  {
    // MAX_MINI_STR_SIZE
        unsigned int valid_size =  value.length() > MAX_MINI_STR_SIZE ? MAX_MINI_STR_SIZE : value.length();
    
    strncpy(device_config_.device_code_type, value.c_str(), valid_size);
    
    //value.toCharArray(
    //    device_config_.device_code_type, valid_size);

    if (value.length() > MAX_MINI_STR_SIZE)
    {
      sprintf(getPrintBuffer(), "cjsp: %d | %s | Possible data truncation.", __LINE__, Device_config_ToString(index));
      this->print(getPrintBuffer());
    }
  }
  break;

  case Device_config_enum::server_host_address_data:
  {
    // MAX_PATH_SIZE
            unsigned int valid_size =  value.length() > MAX_PATH_SIZE ? MAX_PATH_SIZE : value.length();

    value.toCharArray(
        device_config_.server_host_address_data, valid_size);

    if (value.length() > MAX_PATH_SIZE)
    {
      sprintf(getPrintBuffer(), "cjsp: %d | %s | Possible data truncation.", __LINE__, Device_config_ToString(index));
      this->print(getPrintBuffer());
    }
  }
  break;
    // e.g. device1-eews.000webhostapp.com
  case Device_config_enum::server_host_port_data:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.server_host_port_data[0] = value.toInt();
  }
  break;

  case Device_config_enum::host_server_query_path:
  {
    // MAX_PATH_SIZE
    unsigned int valid_size =  value.length() > MAX_PATH_SIZE ? MAX_PATH_SIZE : value.length();

    value.toCharArray(
        device_config_.host_server_query_path, valid_size);

    if (value.length() > MAX_PATH_SIZE)
    {
      sprintf(getPrintBuffer(), "cjsp: %d | %s | Possible data truncation.", __LINE__, Device_config_ToString(index));
      this->print(getPrintBuffer());
    }
  }
  break;

  case Device_config_enum::server_host_address_config:
  {
    // MAX_PATH_SIZE
    unsigned int valid_size =  value.length() > MAX_PATH_SIZE ? MAX_PATH_SIZE : value.length();

    value.toCharArray(
        device_config_.server_host_address_config, valid_size);

    if (value.length() > MAX_PATH_SIZE)
    {
      sprintf(getPrintBuffer(), "cjsp: %d | %s | Possible data truncation.", __LINE__, Device_config_ToString(index));
      this->print(getPrintBuffer());
    }
  }
  break;
  // e.g. device1-eews.000webhostapp.com
  case Device_config_enum::server_host_port_config:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.server_host_port_data[0] = value.toInt();
  }
  break;

  case Device_config_enum::host_config_server_query_path:
  {
    // MAX_PATH_SIZE
    unsigned int valid_size =  value.length() > MAX_PATH_SIZE ? MAX_PATH_SIZE : value.length();

    value.toCharArray(
        device_config_.host_config_server_query_path, valid_size);

    if (value.length() > MAX_PATH_SIZE)
    {
      sprintf(getPrintBuffer(), "cjsp: %d | %s | Possible data truncation.", __LINE__, Device_config_ToString(index));
      this->print(getPrintBuffer());
    }
  }
  break;

  case Device_config_enum::device_min_code_version:
  {
    // MAX_VER_STR_SIZE
    unsigned int valid_size =  value.length() > MAX_VER_STR_SIZE ? MAX_VER_STR_SIZE : value.length();

    value.toCharArray(
        device_config_.device_min_code_version, valid_size);

    if (value.length() > MAX_VER_STR_SIZE)
    {
      sprintf(getPrintBuffer(), "cjsp: %d | %s | Possible data truncation.", __LINE__, Device_config_ToString(index));
      this->print(getPrintBuffer());
    }
  }
  break;
  // 000.000.000-000 or full v000.000.000-000-gf676f80
  case Device_config_enum::device_max_code_version:
  {
    // MAX_VER_STR_SIZE
    unsigned int valid_size =  value.length() > MAX_VER_STR_SIZE ? MAX_VER_STR_SIZE : value.length();

    value.toCharArray(
        device_config_.device_max_code_version, valid_size);

    if (value.length() > MAX_VER_STR_SIZE)
    {
      sprintf(getPrintBuffer(), "cjsp: %d | %s | Possible data truncation.", __LINE__, Device_config_ToString(index));
      this->print(getPrintBuffer());
    }
  }
  break;
  // 000.000.000-000 or full v000.000.000-000-gf676f80
  case Device_config_enum::available_sensor_count:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.available_sensor_count[0] = value.toInt();
  }
  break;

  case Device_config_enum::sensor_current_enabled:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_current_enabled[0] = value.toInt();
  }
  break;
  // bool
  case Device_config_enum::sensor_temperature_enabled:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_temperature_enabled[0] = value.toInt();
  }
  break;
  // bool
  case Device_config_enum::sensor_vibration_enabled:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_temperature_enabled[0] = value.toInt();
  }
  break;
  // bool
  case Device_config_enum::sensor_current_threshold_normal:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_current_threshold_normal[0] = value.toFloat();
  }
  break;

  case Device_config_enum::sensor_current_threshold_alert:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_current_threshold_alert[0] = value.toFloat();
  }
  break;

  case Device_config_enum::sensor_current_threshold_warning:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_current_threshold_warning[0] = value.toFloat();
  }
  break;

  case Device_config_enum::sensor_current_threshold_critical:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_current_threshold_critical[0] = value.toFloat();
  }
  break;

  case Device_config_enum::sensor_vibration_threshold_normal:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_vibration_threshold_normal[0] = value.toFloat();
  }
  break;

  case Device_config_enum::sensor_vibration_threshold_alert:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_vibration_threshold_alert[0] = value.toFloat();
  }
  break;

  case Device_config_enum::sensor_vibration_threshold_warning:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_vibration_threshold_warning[0] = value.toFloat();
  }
  break;

  case Device_config_enum::sensor_vibration_threshold_critical:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_vibration_threshold_critical[0] = value.toFloat();
  }
  break;

  case Device_config_enum::sensor_temperature_threshold_normal:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_temperature_threshold_normal[0] = value.toFloat();
  }
  break;

  case Device_config_enum::sensor_temperature_threshold_alert:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_temperature_threshold_alert[0] = value.toFloat();
  }
  break;

  case Device_config_enum::sensor_temperature_threshold_warning:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_temperature_threshold_warning[0] = value.toFloat();
  }
  break;

  case Device_config_enum::sensor_temperature_threshold_critical:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_temperature_threshold_critical[0] = value.toFloat();
  }
  break;

  case Device_config_enum::sensor_current_sample_time_duration:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_current_sample_time_duration[0] = value.toInt();
  }
  break;

  case Device_config_enum::sensor_vibration_sample_time_duration:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_vibration_sample_time_duration[0] = value.toInt();
  }
  break;

  case Device_config_enum::sensor_temperature_sample_time_duration:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_temperature_sample_time_duration[0] = value.toInt();
  }
  break;

  case Device_config_enum::sensor_current_total_time_duration:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_current_total_time_duration[0] = value.toInt();
  }
  break;

  case Device_config_enum::sensor_vibration_total_time_duration:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_vibration_total_time_duration[0] = value.toInt();
  }
  break;

  case Device_config_enum::sensor_temperature_total_time_duration:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_temperature_total_time_duration[0] = value.toInt();
  }
  break;

  case Device_config_enum::sensor_current_buzzer_light_notify_level:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_current_buzzer_light_notify_level[0] = value.toInt();
  }
  break;

  case Device_config_enum::sensor_temperature_buzzer_light_notify_level:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_temperature_buzzer_light_notify_level[0] = value.toInt();
  }
  break;

  case Device_config_enum::sensor_vibration_buzzer_light_notify_level:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_vibration_buzzer_light_notify_level[0] = value.toInt();
  }
  break;

  case Device_config_enum::development_print_level:
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.sensor_vibration_buzzer_light_notify_level[0] = value.toInt();
  }
  break;  
  
  case Device_config_enum::whether_updated :
  {
    // MAX_SINGLE_CHAR_SIZE
    device_config_.whether_updated[0] = value.toInt();
  }
  break;

  case Device_config_enum::err_msg:
  {
    // MAX_PATH_SIZE
    unsigned int valid_size =  value.length() > MAX_PATH_SIZE ? MAX_PATH_SIZE : value.length();

    value.toCharArray(
        device_config_.err_msg, valid_size);

    if (value.length() > MAX_PATH_SIZE)
    {
      sprintf(getPrintBuffer(), "cjsp: %d | %s | Possible data truncation.", __LINE__, Device_config_ToString(index));
      this->print(getPrintBuffer());
    }
  }
  break;

  default:
  {
    sprintf(getPrintBuffer(), "cjsp: %d | (?? %s ??) | Wrong key received.", __LINE__, Device_config_ToString(index));
    this->print(getPrintBuffer());
  }
  }
  //  if(this->current_key_ == Device_config_ToString(Device_config_enum
  //  ::config_id))
  //  {
  //     device_config_.config_id[0] = value.toInt();
  //  }
}

void ConfigListener::endArray()
{
  //sprintf(getPrintBuffer(), "end array. ");
  //this->print(getPrintBuffer());
}

void ConfigListener::endObject()
{
  //sprintf(getPrintBuffer(), "end object. ");
  //this->print(getPrintBuffer());
}

void ConfigListener::endDocument()
{
  sprintf(getPrintBuffer(), "end document. ");
  this->print(getPrintBuffer());
}

void ConfigListener::startArray()
{
  //sprintf(getPrintBuffer(), "start array. ");
  //this->print(getPrintBuffer());
}

void ConfigListener::startObject()
{
  //sprintf(getPrintBuffer(), "start object. ");
  //this->print(getPrintBuffer());
}

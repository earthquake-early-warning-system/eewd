
#ifndef CONF_JSON_H
#define CONF_JSON_H

#include "JsonListener.h"
#include "config.h"


class ConfigListener: public JsonListener {

  public:
    virtual void whitespace(char c);
  
    virtual void startDocument();

    virtual void key(String key);

    virtual void value(String value);

    virtual void endArray();

    virtual void endObject();

    virtual void endDocument();

    virtual void startArray();

    virtual void startObject();

    Device_config * getDeviceConfigPtr();
    
    private:
    void print(char * _msg);
    String current_key_ = "";
    Device_config device_config_ ;
};

#endif //CONF_JSON_H

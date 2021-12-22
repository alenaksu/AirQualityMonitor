#ifndef SETTINGS_H

#define SETTINGS_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "config.h"

struct SettingValue {
    struct SettingValue* next;
    String value;
    String name;
};

class Settings
{
public:
    void debug();
    void load();
    void save();
    void clear();
    void set(String name, int value);
    void set(String name, float value);
    void set(String name, String  value);
    SettingValue* get(String name);

    bool getBool(String name, bool defaultValue);
    int getInt(String name, int defaultValue);
    float getFloat(String name, float defaultValue);
    String getString(String name, String defaultValue);

    static Settings* getInstance();

private:
    Settings();
    static Settings* instance;
    SettingValue* firstValue;
};

#endif

#include "Settings.h"

Settings* Settings::instance = 0;

Settings::Settings()
{
    if (!SD.begin(SD_CS))
    {
        Serial.println("SD card not detected");
        return;
    }

    if (!SD.exists(SETTINGS_FILE))
    {
        Serial.println("Settings file not found");
    } else
    {
        this->load();
    }

    this->firstValue = NULL;
};

Settings* Settings::getInstance() {
    if (instance == 0) {
        instance = new Settings();
    }
    return instance;
}

void Settings::set(String name, String value)
{
    SettingValue* settingValue = get(name);

    if (!settingValue)
    {
        Serial.println("Creating new setting");
        SettingValue* newValue = (SettingValue*)malloc(sizeof(SettingValue));

        newValue->name = name;
        newValue->value = value;
        newValue->next = firstValue;

        if (firstValue)
            Serial.printf("%s -> %s\n", name.c_str(), firstValue->name.c_str());
        firstValue = newValue;
    } else
    {
        Serial.println("Updating setting");
        settingValue->value = value;
    }
}

void Settings::set(String name, int value)
{
    this->set(name, String(value).c_str());
}

void Settings::set(String name, float value)
{
    this->set(name, String(value).c_str());
}

void Settings::set(String name, long value)
{
    this->set(name, String(value).c_str());
}

void Settings::set(String name, unsigned long value)
{
    this->set(name, String(value).c_str());
}

SettingValue* Settings::get(String name)
{
    Serial.printf("Searching: %s\n", name.c_str());
    SettingValue* current = firstValue;
    while (current)
    {
        if (name.compareTo(current->name) == 0)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

float Settings::getFloat(String name, float defaultValue)
{
    SettingValue* current = get(name);
    if (!current)
        return defaultValue;

    return String(current->value).toFloat();
}

bool Settings::getBool(String name, bool defaultValue)
{
    SettingValue* current = get(name);
    if (!current)
        return defaultValue;

    return String(current->value).compareTo("true") == 0;
}

int Settings::getInt(String name, int defaultValue)
{
    SettingValue* current = get(name);
    if (!current)
        return defaultValue;

    return String(current->value).toInt();
}

String Settings::getString(String name, String defaultValue)
{
    SettingValue* current = get(name);
    if (!current)
        return defaultValue;

    return String(current->value);
}

void Settings::save()
{
    Serial.println("Saving settings...");
    File file = SD.open(SETTINGS_FILE, FILE_WRITE);

    SettingValue* current = this->firstValue;
    while (current != NULL)
    {
        file.printf("%s=%s\n", current->name.c_str(), current->value.c_str());

        current = current->next;
    }

    file.flush();
    file.close();
}

void Settings::debug() {
    SettingValue* current = this->firstValue;

    while (current)
    {

        Serial.print("Name: ");
        Serial.println(current->name);

        Serial.print("Value: ");
        Serial.println(current->value);

        current = current->next;
    }

    Serial.flush();
}

void Settings::clear()
{
    SD.remove(SETTINGS_FILE);

    SettingValue* current = this->firstValue;
    SettingValue* next = NULL;
    while (current != NULL) {
        next = current;
        free(current);
        current = next;
    }

    this->firstValue = NULL;
}

void Settings::load()
{
    File file = SD.open(SETTINGS_FILE);

    Serial.println("Loading settings");
    while (file.available())
    {
        String buffer = file.readStringUntil('\n');
        int sep = strcspn(buffer.c_str(), "=");

        String name = buffer.substring(0, sep);
        String value = buffer.substring(sep + 1);

        Serial.printf("Loaded %s = %s\n", name.c_str(), value.c_str());

        set(name, value);
    }
    file.close();

    Serial.println("Loading settings complete");
};

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#include "Settings.h"
#include "Display.h"
#include "Gui.h"
#include "Sensor.h"
#include "PowerManager.h"

extern "C"
{
#include "gpio.h"
}
extern "C"
{
#include "user_interface.h"
}

// ADC_MODE(ADC_VCC);

    // WiFiManager wifi = WiFiManager();
Display display = Display(TFT_CS, TFT_DC, STMPE_CS);
Sensor sensor = Sensor();
Gui gui = Gui(&display.driver);

void setup()
{
    // start the serial connection
    Serial.begin(115200);

    Serial.println("Initializing display...");
    display.begin();

    Serial.println("Initializing gui...");
    gui.begin();

    Serial.println("Initializing sensors...");
    sensor.begin();

    PowerManager::setWakeUpCallback([](WakeUpReason reason) {
        if (reason == WakeUpReason::interrupt) {
            display.turnOn(DISPLAY_TIMEOUT);
        }
        });

    // wifi.autoConnect("AirMonitor");
    // wifi.setAPCallback(handleAPConfig);
    // wifi.setSaveConfigCallback(handleWiFiSaveConfig);
    display.turnOn(DISPLAY_TIMEOUT);
}

void loop()
{
    Serial.println("Running...");

    Serial.println("Reading sensor data...");
    SensorData data = sensor.read();

    Serial.println("Drawing gui...");
    gui.draw(data);

    Serial.println("Waiting for display...");
    display.waitForTimeout();

    Serial.println("Good night :)");
    PowerManager::sleep(SLEEP_TIMEOUT);
}

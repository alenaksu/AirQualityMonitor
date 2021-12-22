#ifndef POWER_MANAGER_H

#define POWER_MANAGER_H

#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#define WAKE_PIN 1

enum WakeUpReason {
    timeout,
    interrupt
};

typedef void (*WakeUpCallback)(WakeUpReason reason);

namespace PowerManager {
    void sleep();
    void sleep(uint32 sleepTime);
    void wakeUp();
    void setWakeUpCallback(WakeUpCallback callback);
}

#endif

#include "PowerManager.h"

extern os_timer_t* timer_list;

namespace PowerManager {
    WakeUpCallback wakeCallback;
    os_timer_t* lastTimerList;

    void setWakeUpCallback(WakeUpCallback cb) {
        wakeCallback = cb;
    }

    void sleep(uint32 sleepTime) {
        Serial.flush();

        pinMode(WAKE_PIN, FUNCTION_3);
        pinMode(WAKE_PIN, INPUT);

        // for timer-based light sleep to work, the os timers need to be disconnected
        lastTimerList = timer_list;
        timer_list = nullptr;

        wifi_station_disconnect();
        wifi_set_opmode_current(NULL_MODE);
        wifi_fpm_set_sleep_type(LIGHT_SLEEP_T); // set sleep type, the above
        wifi_fpm_open();                        // Enables force sleep

        // wifi_enable_gpio_wakeup(GPIO_ID_PIN(WAKE_PIN), GPIO_PIN_INTR_HILEVEL); // GPIO_ID_PIN(2) corresponds to GPIO2 on ESP8266-01 , GPIO_PIN_INTR_LOLEVEL for a logic low, can also do other interrupts, see gpio.h above
        gpio_pin_wakeup_enable(GPIO_ID_PIN(WAKE_PIN), GPIO_PIN_INTR_HILEVEL);

        wifi_fpm_set_wakeup_cb(wakeUp);
        wifi_fpm_do_sleep(sleepTime * 1000);     // Sleep for longest possible time
        delay(sleepTime + 1);
    };

    void sleep() {
        sleep(0xFFFFFFF);
    };

    void wakeUp() {
        Serial.println("Waking up!");
        Serial.flush();

        bool iswakePinHigh = digitalRead(WAKE_PIN) == HIGH;

        timer_list = lastTimerList;
        lastTimerList = nullptr;

        pinMode(WAKE_PIN, FUNCTION_0);

        // wifi_disable_gpio_wakeup();
        gpio_pin_wakeup_disable();

        wifi_fpm_close();               //	disable	force	sleep	function
        // wifi_set_opmode(STATION_MODE); //	set	station	mode
        // wifi_station_connect();        //	connect	to	AP

        if (wakeCallback) (*wakeCallback)(iswakePinHigh ? WakeUpReason::interrupt : WakeUpReason::timeout);
    };
}

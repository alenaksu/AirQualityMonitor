## Components

-   [Adafruit Feather HUZZAH ESP8266](https://www.adafruit.com/product/2821)
-   [Adafruit 2.4" TFT FeatherWing](https://learn.adafruit.com/adafruit-2-4-tft-touch-screen-featherwing)
-   [SGP30 VOC eCO2 sensor](https://learn.adafruit.com/adafruit-sgp30-gas-tvoc-eco2-mox-sensor)
-   [BME280 I2C or SPI Temperature Humidity Pressure Sensor](https://www.adafruit.com/product/2652)

## SGP30

### Calibrating Baseline

The SGP30 sensor will re-calibrate its baseline each time it is powered on. During the first power-up this will take up to 12 hours. Exposing to outside air for at least 10 minutes cumulative time is advised during the calibration period.

For best performance and faster startup times, the current baseline needs to be persistently stored on the device before shutting it down and set again accordingly after boot up. It implies that if the sensor reboots at a time when the air is less clean than normal, the values will have a constant offset and cannot be compared to the values before the last boot.

Using the store_baseline option will automatically store the baseline values after calibration or when it is updated during operation. When booting up, the stored values will then be (re)applied in the sensor. Stored baselines are cleared after OTA.

Another method is to manually specify the baseline values in the configuration file. To do this, let the sensor boot up with no baseline set and let the sensor calibrate itself. After around 12 hours you can then view the remote logs on the ESP. The nexttime the sensor is read out, you will see a log message with something like Current eCO2 baseline: 0x86C5, TVOC baseline: 0x8B38.

Another way to obtain the baseline values is to configure the eco2 and TVOC baseline value sensors. Values will be published to your Home Automation system. Convert the decimal value to hex value before use (e.g. 37577 –> 0x92C9)

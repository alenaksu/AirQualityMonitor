#ifndef SENSOR_H

#define SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_SGP30.h>
#include "Settings.h"
#include "utils.h"

// SGP30 Sensor definitions
#define SENSOR_BASELINE_UPDATE_FREQ 36e5     // 1 hour
#define SENSOR_BASELINE_CALIBRATION_DURATION 4.32e7  // 12 hours
#define SENSOR_BASELINE_VALIDITY 6.048e8     // 1 week

// BME280 Sensor Definitions
#define SENSOR_SEALEVELPRESSURE_HPA (1013.25)
#define SENSOR_BME_ADDR 0x76

// Settings
#define SENSOR_SETTING_BASELINE_UPDATE "baseline_last_update"
#define SENSOR_SETTING_ECO2_BASELINE "eco2_baseline"
#define SENSOR_SETTING_TVOC_BASELINE "tvoc_baseline"

struct SensorData
{
    float temperature;
    float pressure;
    float altitude;
    float humidity;
    float tvoc;
    float eco2;
};

class Sensor
{
private:
    Adafruit_BME280 bme;
    Adafruit_SGP30 sgp;
    float lastRead = -1;
    float firstRead = -1;
    void storeBaseline();
    bool needsCalibration();

public:
    Sensor() : bme(), sgp() {};
    bool begin();
    SensorData read();
};

#endif

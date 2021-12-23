#include "Sensor.h"

bool Sensor::begin()
{
    Settings* settings = Settings::getInstance();

    if (!bme.begin(SENSOR_BME_ADDR))
    {
        Serial.println("Could not find a valid BME280 sensor");
        return false;
    } else
    {
        Serial.print("Found BME280 serial #");
        Serial.println(bme.sensorID(), HEX);
    }

    if (!sgp.begin())
    {
        Serial.println("Could not find a valid SGP30 sensor");
        return false;
    } else
    {
        Serial.print("Found SGP30 serial #");
        Serial.print(sgp.serialnumber[0], HEX);
        Serial.print(sgp.serialnumber[1], HEX);
        Serial.println(sgp.serialnumber[2], HEX);
    }

    // Set up the BME280 sensor
    bme.setSampling(
        Adafruit_BME280::MODE_FORCED,
        Adafruit_BME280::SAMPLING_X1,
        Adafruit_BME280::SAMPLING_X1,
        Adafruit_BME280::SAMPLING_X1,
        Adafruit_BME280::FILTER_OFF,
        Adafruit_BME280::STANDBY_MS_0_5);

    if (!needsCalibration()) {
        // Set up the SGP30 sensor
        uint16_t tvocBaseline = (uint16_t)settings->getInt(SENSOR_SETTING_TVOC_BASELINE, -1);
        uint16_t eco2Baseline = (uint16_t)settings->getInt(SENSOR_SETTING_ECO2_BASELINE, -1);

        if (tvocBaseline != -1 && eco2Baseline != -1) {
            Serial.printf("Restoring baselines. eco2: %d, tvoc: %d\n", eco2Baseline, tvocBaseline);
            sgp.setIAQBaseline(eco2Baseline, tvocBaseline);
        }
    }

    return sgp.IAQinit();
}

bool Sensor::needsCalibration()
{
    // TODO time handling
    Settings* settings = Settings::getInstance();
    unsigned long now = millis();
    float lastUpdate = settings->getFloat(SENSOR_SETTING_BASELINE_UPDATE, -1);
    bool needsCalibration =
        lastUpdate == -1                                     // No baseline present
        || now < lastUpdate                                  // Device has been resetted
        || now < SENSOR_BASELINE_CALIBRATION_DURATION;       // Calibration not completed

    return needsCalibration;
}

void Sensor::storeBaseline()
{
    if (needsCalibration()) {
        Serial.println("Sensor is calibrating");
        return;
    }

    Settings* settings = Settings::getInstance();
    unsigned long now = millis();
    if (now - lastRead > SENSOR_BASELINE_UPDATE_FREQ)
    {
        lastRead = now;

        uint16_t tvocBaseline;
        uint16_t eco2Baseline;
        if (sgp.getIAQBaseline(&eco2Baseline, &tvocBaseline))
        {
            Serial.printf("Saving baseline: %d, %d\n", eco2Baseline, tvocBaseline);
            settings->set(SENSOR_SETTING_BASELINE_UPDATE, now);
            settings->set(SENSOR_SETTING_ECO2_BASELINE, eco2Baseline);
            settings->set(SENSOR_SETTING_TVOC_BASELINE, tvocBaseline);
            settings->save();
        }
    }
}

SensorData Sensor::read()
{
    SensorData data;

    bme.takeForcedMeasurement();

    data.temperature = bme.readTemperature();
    data.altitude = bme.readAltitude(SENSOR_SEALEVELPRESSURE_HPA);
    data.humidity = bme.readHumidity();
    data.pressure = bme.readPressure() / 100.0F;

    sgp.setHumidity(computeAbsoluteHumidity(data.temperature, data.humidity));
    sgp.IAQmeasure();
    data.tvoc = sgp.TVOC;
    data.eco2 = sgp.eCO2;

    if (firstRead == -1)
        firstRead = millis();

    storeBaseline();

    return data;
}

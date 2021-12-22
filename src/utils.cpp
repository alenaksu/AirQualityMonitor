#include <Arduino.h>
#include "utils.h"

/**
 * @brief Copmute the water vapour saturation pressure
 * 
 * @param temperature 
 * @return float 
 */
float computePws(float temperature)
{
  float m = 7.591386;
  float Tn = 240.7263;
  float A = 6.116441;

  return A * pow(10, ((m * temperature) / (temperature + Tn)));
}

float computeAbsoluteHumidity(float temperature, float humidity)
{
  float C = 2.16679;
  float Pw = computePws(temperature) * (humidity / 100); // hPA
  float A = C * (Pw * 100) / (273.15 + temperature);

  return A;
}
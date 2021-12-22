#ifndef DISPLAY_H

#define DISPLAY_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_STMPE610.h>
#include <MiniGrafx.h>

#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 320

class Display {
private:
    float displayTimeout = -1;
    bool displayActive;

public:
    Display(int8_t tftCS, int8_t tftDC, uint8_t touchPin) : touch(touchPin), driver(tftCS, tftDC) {};
    Adafruit_STMPE610 touch;
    ILI9341_SPI driver;
    bool begin();
    void turnOff();
    void turnOn();
    void turnOn(float timeout);
    void waitForTimeout();
};

#endif

#ifndef GUI_H

#define GUI_H

#include <Adafruit_GFX.h>
#include <MiniGrafx.h>
#include <Sensor.h>

#include "Display.h"
#include "fonts/fonts.h"

extern uint16_t GuiPalette[4];

// Limited to 4 colors due to memory constraints (2^2 = 4 colours)
#define GUI_BITS_PER_PIXEL 2

class Gui {
private:
    MiniGrafx gfx;

public:
    Gui(DisplayDriver* driver) : gfx(driver, GUI_BITS_PER_PIXEL, GuiPalette) {};

    bool begin();
    void draw(SensorData &data);
};

#endif

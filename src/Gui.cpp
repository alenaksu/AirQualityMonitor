#include "Gui.h"

uint16_t GuiPalette[] = {
    ILI9341_BLACK,  // 0
    ILI9341_WHITE,  // 1

    ILI9341_YELLOW, // 2
    ILI9341_RED     // 3
};

bool Gui::begin() {
    delay(100);
    gfx.init();
    gfx.setRotation(0);
    gfx.commit();

    return true;
}

void Gui::draw(SensorData &data) {
    uint16_t w = gfx.getWidth();
    uint16_t padding = 10;

    gfx.clear();
    gfx.fillBuffer(0);

    gfx.setColor(1);

    gfx.setTextAlignment(TEXT_ALIGN_LEFT);
    gfx.setFont(ArialMT_Plain_16);
    gfx.drawString(padding, 54, "eCO2");

    gfx.setFont(DSEG7_Classic_Regular_48);
    gfx.drawString(60, 10 + padding, String(data.eco2, 0));

    gfx.setFont(ArialMT_Plain_16);
    gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
    gfx.drawString(w - padding, 54, "ppm");

    const String labels[4] = {
        "TVOC",
        "Temp",
        "Humidity",
        "Pressure"
    };
    const String values[4] = {
        String(data.tvoc, 0) + " ppb",
        String(data.temperature, 0) + " °C",
        String(data.humidity, 0) + " %",
        String(data.pressure, 0) + " hPa"
    };

    gfx.setFont(ArialMT_Plain_24);

    int y = 80;
    int linePadding = 15;
    for (int i = 0; i < 4; i++) {
        gfx.drawHorizontalLine(0, y, w);

        y += linePadding;

        gfx.setTextAlignment(TEXT_ALIGN_LEFT);
        gfx.drawString(padding, y, labels[i]);

        gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
        gfx.drawString(w - padding, y, values[i]);

        y += 28; //  font height

        y += linePadding;
    }

    gfx.commit();
}



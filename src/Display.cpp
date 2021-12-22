#include "Display.h"


bool Display::begin() {
    delay(100);
    if (!touch.begin())
    {
        Serial.println("STMPE not found?");
        while(1);
        return false;
    }

    // this pin (#2) is connected to the STMPE IRQ pin
    // pinMode(STMPE_IRQ, INPUT_PULLUP);

    // tell the STMPE to use 'low' level as 'touched' indicator
    touch.writeRegister8(STMPE_INT_CTRL, STMPE_INT_CTRL_POL_HIGH | STMPE_INT_CTRL_ENABLE);

    // we'll use STMPE's GPIO 2 for backlight control
    touch.writeRegister8(STMPE_GPIO_DIR, bit(2));
    touch.writeRegister8(STMPE_GPIO_ALT_FUNCT, bit(2));

    // backlight on
    touch.writeRegister8(STMPE_GPIO_SET_PIN, bit(2));

    // turnOn();

    return true;
}

void Display::turnOn(float timeout) {
    displayTimeout = timeout;

    if (displayActive) return;

    // Backlight on
    touch.writeRegister8(STMPE_GPIO_SET_PIN, bit(2));

    // Send command to wakeup the display.
    driver.writecommand(ILI9341_SLPOUT);
    driver.writecommand(ILI9341_DISPON);

    displayActive = true;
}

void Display::turnOn() {
    turnOn(-1);
}

void Display::turnOff() {
    if (!displayActive) return;

    displayTimeout = -1;

    // Backlight off
    touch.writeRegister8(STMPE_GPIO_CLR_PIN, bit(2));
    touch.writeRegister8(STMPE_INT_STA, 0xFF);

    // Send command to put the display to sleep.
    driver.writecommand(ILI9341_SLPIN);
    driver.writecommand(ILI9341_DISPOFF);
    delay(120);

    displayActive = false;
}

void Display::waitForTimeout() {
    if (displayActive && displayTimeout != -1) {
        delay(displayTimeout);
        turnOff();
    }
}

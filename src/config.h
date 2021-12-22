#ifndef CONFIG_H

#define CONFIG_H

// Pins for the ILI9341
#define TFT_DC 15
#define TFT_CS 0
#define TFT_LED 5

#define SD_CS 2

#define DISPLAY_TIMEOUT 10000

// pins for the touchscreen
#define STMPE_CS 16
#define STMPE_IRQ 4

#define SETTINGS_FILE "settings.txt"

// Delay between sensor reads, in seconds
#define SLEEP_TIMEOUT 2.5 * 60 * 1000 // 2.5 minutes

#endif

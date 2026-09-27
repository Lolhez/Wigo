#pragma once

// WIGO hardware and capacity settings.
// Edit this file to match your board. Check the board pinout before wiring.

#define WIGO_GPIO_BUTTON_UP 25
#define WIGO_GPIO_BUTTON_OK 26
#define WIGO_GPIO_BUTTON_DOWN 27
#define WIGO_GPIO_BATTERY_ADC 34
#define WIGO_GPIO_OLED_SDA 21
#define WIGO_GPIO_OLED_SCL 22

#define WIGO_OLED_I2C_ADDRESS 0x3C
#define WIGO_OLED_WIDTH 128
#define WIGO_OLED_HEIGHT 64
#define WIGO_OLED_ROTATION 2  // 180 degrees; change to 0 if your display is upright.

// Four is the conservative default. Larger values require a real device load
// test and may require raising the WebSockets library client limit globally.
#define WIGO_MAX_PLAYERS 4

#if WIGO_MAX_PLAYERS < 1 || WIGO_MAX_PLAYERS > 10
#error "WIGO_MAX_PLAYERS must be between 1 and 10 (ESP32 SoftAP station limit)."
#endif

#if defined(WEBSOCKETS_SERVER_CLIENT_MAX) && WEBSOCKETS_SERVER_CLIENT_MAX < WIGO_MAX_PLAYERS
#error "WEBSOCKETS_SERVER_CLIENT_MAX must be at least WIGO_MAX_PLAYERS."
#endif

// The WebSockets library defaults to five clients. Do not define its limit in
// this sketch header alone: its .cpp files must use the same class layout.
#if !defined(WEBSOCKETS_SERVER_CLIENT_MAX) && WIGO_MAX_PLAYERS > 5
#error "For more than 5 players, define WEBSOCKETS_SERVER_CLIENT_MAX globally for both the sketch and WebSockets library."
#endif

#define WIGO_AP_PASSWORD "wigo-game"

// Battery divider: battery positive -> R1 -> ADC pin -> R2 -> GND.
// Replace these with the measured resistor values in your device.
#define WIGO_BATTERY_R1_OHMS 100000.0f
#define WIGO_BATTERY_R2_OHMS 100000.0f

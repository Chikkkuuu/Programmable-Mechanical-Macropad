#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "USBHIDKeyboard.h"

// --- I2C OLED DISPLAY ---
#define I2C_ADDR     0x3C 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET   -1 

// --- PINS (ESP32-S3) ---
#define POT_PIN    3   
#define ROTARY_CLK 1   
#define ROTARY_DT  2   
#define ROTARY_SW  42  
#define STATUS_LED 38

const int BTN_PINS[9] = {4, 5, 6, 7, 15, 16, 17, 18, 8};
const int MODE_THRESHOLDS[4] = {820, 1640, 2460, 3280};

// --- MODIFIER BITMASKS (Sum these up for shortcuts) ---
#define MOD_NONE   0
#define MOD_LCTRL  1
#define MOD_LSHIFT 2
#define MOD_LALT   4
#define MOD_LGUI   8   // Windows Key / Command
#define MOD_RCTRL  16
#define MOD_RSHIFT 32
#define MOD_RALT   64
#define MOD_RGUI   128

// --- DATA STRUCTURES ---
struct KeyBinding { 
  uint8_t modifierMask; // Sum of modifiers
  uint8_t key;          // ASCII or HID Code
};

enum EncoderAction { ENC_SCROLL, ENC_ZOOM, ENC_KEY };
struct EncoderConfig { 
  EncoderAction action; 
  uint8_t cwMod; uint8_t cwKey; 
  uint8_t ccwMod; uint8_t ccwKey; 
};

// Notification is now an overlay, not a widget type
enum WidgetType { WIDGET_STATUS, WIDGET_MEDIA, WIDGET_TIME, WIDGET_WEATHER };

// --- MUTABLE STATE (Global Variables) ---
extern KeyBinding KEY_MAP[5][9]; 
extern char BTN_LABELS[5][9][10]; 
extern char MODE_NAMES[5][12];
extern EncoderConfig ENCODER_MAP[5];
extern WidgetType SCREEN_WIDGET[5];
extern int currentMode;

#endif
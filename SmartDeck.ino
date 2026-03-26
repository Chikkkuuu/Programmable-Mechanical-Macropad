#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "USB.h"
#include "USBHIDMouse.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"

USBHIDMouse Mouse;
USBHIDKeyboard Keyboard;
USBHIDConsumerControl ConsumerControl;

#include "Config.h"

// --- GLOBAL VARIABLES (Storage Holders) ---
KeyBinding KEY_MAP[5][9];
char BTN_LABELS[5][9][10];
char MODE_NAMES[5][12] = {"GAMING", "CODING", "EDITING", "VIDEO", "STREAM"};
EncoderConfig ENCODER_MAP[5];
WidgetType SCREEN_WIDGET[5] = {WIDGET_STATUS, WIDGET_STATUS, WIDGET_STATUS, WIDGET_STATUS, WIDGET_STATUS};
int currentMode = 0;

#include "Display.h"
#include "Actions.h"
#include "Inputs.h"
#include "Storage.h"

void setup() {
  Serial.begin(115200);

  // 1. Initialize Defaults (Fallback if storage is empty)
  for(int m=0; m<5; m++) {
    for(int b=0; b<9; b++) {
      KEY_MAP[m][b] = {0, 0}; 
      sprintf(BTN_LABELS[m][b], "BTN %d", b+1);
    }
    ENCODER_MAP[m] = {ENC_SCROLL, 0, 0, 0, 0};
  }

  // 2. Load Settings from Memory (Overrides defaults)
  initStorage();
  loadSettings(); 

  // 3. Init Hardware
  initDisplay();
  initInputs();
  initUSB(); 
}

void loop() {
  checkSerial(); 
  checkPotentiometer();
  checkButtons();
  checkEncoder();
  tickDisplay(); 
  delay(1); 
}
#ifndef ACTIONS_H
#define ACTIONS_H

#include "Config.h"

extern USBHIDMouse Mouse;
extern USBHIDKeyboard Keyboard;
extern USBHIDConsumerControl ConsumerControl;

void initUSB() {
  USB.begin();
  Mouse.begin();
  Keyboard.begin();
  ConsumerControl.begin();
}

// --- HELPER: Apply Bitmask Modifiers ---
void applyModifiers(uint8_t mask, bool press) {
  if (mask & MOD_LCTRL)  press ? Keyboard.press(KEY_LEFT_CTRL)   : Keyboard.release(KEY_LEFT_CTRL);
  if (mask & MOD_LSHIFT) press ? Keyboard.press(KEY_LEFT_SHIFT)  : Keyboard.release(KEY_LEFT_SHIFT);
  if (mask & MOD_LALT)   press ? Keyboard.press(KEY_LEFT_ALT)    : Keyboard.release(KEY_LEFT_ALT);
  if (mask & MOD_LGUI)   press ? Keyboard.press(KEY_LEFT_GUI)    : Keyboard.release(KEY_LEFT_GUI);
  if (mask & MOD_RCTRL)  press ? Keyboard.press(KEY_RIGHT_CTRL)  : Keyboard.release(KEY_RIGHT_CTRL);
  if (mask & MOD_RSHIFT) press ? Keyboard.press(KEY_RIGHT_SHIFT) : Keyboard.release(KEY_RIGHT_SHIFT);
  if (mask & MOD_RALT)   press ? Keyboard.press(KEY_RIGHT_ALT)   : Keyboard.release(KEY_RIGHT_ALT);
  if (mask & MOD_RGUI)   press ? Keyboard.press(KEY_RIGHT_GUI)   : Keyboard.release(KEY_RIGHT_GUI);
}

void sendMacro(uint8_t modifierMask, uint8_t key) {
  applyModifiers(modifierMask, true);
  if (key != 0) Keyboard.press(key);
  delay(10);
  Keyboard.releaseAll();
}

void handleEncoder(int mode, bool clockwise) {
  EncoderConfig cfg = ENCODER_MAP[mode];
  switch(cfg.action) {
    case ENC_SCROLL:
      Mouse.move(0, 0, clockwise ? 1 : -1);
      break;
    case ENC_ZOOM:
      Keyboard.press(KEY_LEFT_CTRL);
      Mouse.move(0, 0, clockwise ? 1 : -1);
      Keyboard.releaseAll();
      break;
    case ENC_KEY:
      if (clockwise) sendMacro(cfg.cwMod, cfg.cwKey);
      else sendMacro(cfg.ccwMod, cfg.ccwKey);
      break;
  }
}

void handlePress(int mode, int btnIndex) {
  KeyBinding bind = KEY_MAP[mode][btnIndex];
  applyModifiers(bind.modifierMask, true);
  if (bind.key != 0) Keyboard.press(bind.key);
}

void handleRelease(int mode, int btnIndex) {
  KeyBinding bind = KEY_MAP[mode][btnIndex];
  if (bind.key != 0) Keyboard.release(bind.key);
  applyModifiers(bind.modifierMask, false);
  if (bind.modifierMask == 0 && bind.key == 0) Keyboard.releaseAll();
}

#endif
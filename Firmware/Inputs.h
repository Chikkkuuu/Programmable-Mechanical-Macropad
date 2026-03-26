#ifndef INPUTS_H
#define INPUTS_H

#include "Config.h"
#include "Display.h"
#include "Actions.h"
#include "Storage.h"

int lastModeCheck = -1;
int lastEncoderPos = HIGH;
unsigned long lastDebounce = 0;
bool btnState[9] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

void initInputs() {
  pinMode(POT_PIN, INPUT);
  pinMode(ROTARY_CLK, INPUT);
  pinMode(ROTARY_DT, INPUT);
  pinMode(ROTARY_SW, INPUT_PULLUP);
  pinMode(STATUS_LED, OUTPUT);
  for(int i=0; i<9; i++) pinMode(BTN_PINS[i], INPUT_PULLUP);
}

void checkSerial() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('|');
    
    if (cmd == "REMAP") {
      int m = Serial.readStringUntil('|').toInt();
      int b = Serial.readStringUntil('|').toInt();
      int modMask = Serial.readStringUntil('|').toInt(); 
      int k = Serial.readStringUntil('\n').toInt();
      KEY_MAP[m][b] = {(uint8_t)modMask, (uint8_t)k};
      saveKeyMap();
      Serial.println("OK|REMAP");
    }
    else if (cmd == "BTNLABEL") {
      int m = Serial.readStringUntil('|').toInt();
      int b = Serial.readStringUntil('|').toInt();
      String lbl = Serial.readStringUntil('\n');
      lbl.toCharArray(BTN_LABELS[m][b], 10);
      saveLabels();
      Serial.println("OK|BTNLABEL");
    }
    else if (cmd == "MODENAME") {
      int m = Serial.readStringUntil('|').toInt();
      String name = Serial.readStringUntil('\n');
      name.toCharArray(MODE_NAMES[m], 12);
      saveModes();
      Serial.println("OK|MODENAME");
    }
    else if (cmd == "ENCMAP") {
      int m = Serial.readStringUntil('|').toInt();
      int act = Serial.readStringUntil('|').toInt(); 
      int cwM = Serial.readStringUntil('|').toInt();
      int cwK = Serial.readStringUntil('|').toInt();
      int ccwM = Serial.readStringUntil('|').toInt();
      int ccwK = Serial.readStringUntil('\n').toInt();
      ENCODER_MAP[m] = {(EncoderAction)act, (uint8_t)cwM, (uint8_t)cwK, (uint8_t)ccwM, (uint8_t)ccwK};
      saveEncoder();
      Serial.println("OK|ENCMAP");
    }
    else if (cmd == "WIDGET") { 
      int m = Serial.readStringUntil('|').toInt();
      int w = Serial.readStringUntil('\n').toInt();
      SCREEN_WIDGET[m] = (WidgetType)w;
      saveWidgets();
      Serial.println("OK|WIDGET");
    }
    else if (cmd == "RESET") {
      wipeStorage();
      ESP.restart();
    }
    // --- LIVE DATA COMMANDS ---
    else if (cmd == "NOTIF") {
      String app = Serial.readStringUntil('|');
      String body = Serial.readStringUntil('\n');
      setNotification(app, body);
      Serial.println("OK|NOTIF");
    }
    else if (cmd == "MEDIA") {
      String t = Serial.readStringUntil('|');
      String a = Serial.readStringUntil('\n');
      setMedia(t, a);
      Serial.println("OK|MEDIA");
    }
    else if (cmd == "TIME") {
      String t = Serial.readStringUntil('\n');
      setTime(t);
    }
    else if (cmd == "WEATHER") {
      String c = Serial.readStringUntil('|');
      String t = Serial.readStringUntil('|');
      String s = Serial.readStringUntil('\n');
      setWeather(c, t, s);
    }
  }
}

void checkPotentiometer() {
  int raw = analogRead(POT_PIN);
  int newMode = 0;
  if (raw < MODE_THRESHOLDS[0]) newMode = 0;
  else if (raw < MODE_THRESHOLDS[1]) newMode = 1;
  else if (raw < MODE_THRESHOLDS[2]) newMode = 2;
  else if (raw < MODE_THRESHOLDS[3]) newMode = 3;
  else newMode = 4;
  if (newMode != lastModeCheck) {
    currentMode = newMode;
    lastModeCheck = newMode;
    delay(100);
  }
}

void checkEncoder() {
  int currentEncoderPos = digitalRead(ROTARY_CLK);
  if (currentEncoderPos != lastEncoderPos && currentEncoderPos == LOW) {
    if (millis() - lastDebounce > 5) {
      bool clockwise = (digitalRead(ROTARY_DT) != currentEncoderPos);
      handleEncoder(currentMode, clockwise);
      lastDebounce = millis();
    }
  }
  lastEncoderPos = currentEncoderPos;
}

void checkButtons() {
  for(int i=0; i<9; i++) {
    bool isPressed = (digitalRead(BTN_PINS[i]) == LOW);
    if (isPressed && btnState[i] == HIGH) {
      handlePress(currentMode, i);
      btnState[i] = LOW;
    } else if (!isPressed && btnState[i] == LOW) {
      handleRelease(currentMode, i);
      btnState[i] = HIGH;
    }
  }
}
#endif
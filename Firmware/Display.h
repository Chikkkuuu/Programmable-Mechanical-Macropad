#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "Config.h"

#define SDA_PIN 48
#define SCL_PIN 47

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Overlay Data
String notifApp = "";
String notifBody = "";
unsigned long notifStartTime = 0;
bool notifActive = false;

// Widget Data
String timeStr = "--:--";
String cityStr = "Loading";
String tempStr = "--";
String condStr = "";
String mediaTitle = "Nothing Playing";
String mediaArtist = "Paused";

void initDisplay() {
  // CRITICAL FIX FOR WOKWI / S3
  Wire.setPins(SDA_PIN, SCL_PIN);
  Wire.begin(); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDR)) {
    pinMode(STATUS_LED, OUTPUT);
    while(1) { digitalWrite(STATUS_LED, HIGH); delay(100); digitalWrite(STATUS_LED, LOW); delay(100); }
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("SMART DECK V6"));
  display.println(F("SYSTEM READY"));
  display.display();
}

void setNotification(String app, String body) {
  notifApp = app;
  notifBody = body;
  notifStartTime = millis();
  notifActive = true;
  digitalWrite(STATUS_LED, HIGH);
}
void setMedia(String t, String a) { mediaTitle = t; mediaArtist = a; }
void setTime(String t) { timeStr = t; }
void setWeather(String c, String t, String s) { cityStr = c; tempStr = t; condStr = s; }

// --- RENDERERS ---
void _renderNotifAlert() {
  display.fillRect(0, 0, 128, 14, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(2, 3); display.print(notifApp);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  String textToShow = notifBody;
  if (textToShow.length() > 42) textToShow = textToShow.substring(0, 39) + "...";
  display.print(textToShow);
  int w = map(millis() - notifStartTime, 0, 5000, 128, 0);
  display.fillRect(0, 60, w, 4, SSD1306_WHITE);
}

void _renderMedia() {
  display.setTextSize(1); display.setCursor(0, 0); display.print(F("NOW PLAYING"));
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  display.setTextSize(2); display.setCursor(0, 15);
  String t = mediaTitle;
  if (t.length() > 10) t = t.substring(0, 9) + ".";
  display.print(t);
  display.setTextSize(1); display.setCursor(0, 35); display.print(mediaArtist);
  display.drawRect(0, 50, 100, 6, SSD1306_WHITE);
  display.fillRect(0, 50, 60, 6, SSD1306_WHITE); 
  display.setCursor(105, 50); display.print(F(">"));
}

void _renderStatus() {
  display.setTextSize(2); display.setCursor(0, 0); display.print(MODE_NAMES[currentMode]);
  display.setTextSize(1);
  display.setCursor(0, 25); display.print(F("Btn 1: ")); display.print(BTN_LABELS[currentMode][0]);
  display.setCursor(0, 35); display.print(F("Btn 2: ")); display.print(BTN_LABELS[currentMode][1]);
}

void _renderTime() {
  display.setTextSize(3);
  int16_t x1, y1; uint16_t w, h;
  display.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((128-w)/2, 20); display.print(timeStr);
}

void _renderWeather() {
  display.setTextSize(1); display.setCursor(0,0); display.print(cityStr);
  display.setTextSize(2); display.setCursor(0, 20); display.print(tempStr + "C");
  display.setTextSize(1); display.setCursor(0, 45); display.print(condStr);
}

void tickDisplay() {
  display.clearDisplay();
  if (notifActive) {
    if (millis() - notifStartTime > 5000) { notifActive = false; digitalWrite(STATUS_LED, LOW); } 
    else { _renderNotifAlert(); }
  } else {
    switch(SCREEN_WIDGET[currentMode]) {
      case WIDGET_TIME:    _renderTime(); break;
      case WIDGET_WEATHER: _renderWeather(); break;
      case WIDGET_MEDIA:   _renderMedia(); break;
      default:             _renderStatus(); break;
    }
  }
  display.display();
}

#endif
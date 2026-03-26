#ifndef STORAGE_H
#define STORAGE_H

#include <Preferences.h>
#include "Config.h"

Preferences preferences;

void initStorage() {
  preferences.begin("smartdeck", false); // Open storage in R/W mode
}

void loadSettings() {
  if (preferences.isKey("keymap"))  preferences.getBytes("keymap", KEY_MAP, sizeof(KEY_MAP));
  if (preferences.isKey("labels"))  preferences.getBytes("labels", BTN_LABELS, sizeof(BTN_LABELS));
  if (preferences.isKey("modes"))   preferences.getBytes("modes", MODE_NAMES, sizeof(MODE_NAMES));
  if (preferences.isKey("encmap"))  preferences.getBytes("encmap", ENCODER_MAP, sizeof(ENCODER_MAP));
  if (preferences.isKey("widgets")) preferences.getBytes("widgets", SCREEN_WIDGET, sizeof(SCREEN_WIDGET));
}

// --- SAVE FUNCTIONS ---
void saveKeyMap()  { preferences.putBytes("keymap", KEY_MAP, sizeof(KEY_MAP)); }
void saveLabels()  { preferences.putBytes("labels", BTN_LABELS, sizeof(BTN_LABELS)); }
void saveModes()   { preferences.putBytes("modes", MODE_NAMES, sizeof(MODE_NAMES)); }
void saveEncoder() { preferences.putBytes("encmap", ENCODER_MAP, sizeof(ENCODER_MAP)); }
void saveWidgets() { preferences.putBytes("widgets", SCREEN_WIDGET, sizeof(SCREEN_WIDGET)); }

void wipeStorage() { preferences.clear(); }

#endif
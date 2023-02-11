#pragma once

#ifndef SETUP_FUNCTION_H
#define SETUP_FUNCTION_H

#include <Config.h>

static inline void tictactoe_setup() {
#if USE_SERIAL
  Serial.begin(9600);
#endif
  for (uint8_t i = 0; i < PIN_COUNT; i++) {
    pinMode(PINS[i], OUTPUT);
  }
  for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
    pinMode(BUTTONS[i], INPUT_PULLUP);
  }
}

#endif
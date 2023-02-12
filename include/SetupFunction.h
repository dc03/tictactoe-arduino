#pragma once

#ifndef SETUP_FUNCTION_H
#define SETUP_FUNCTION_H

#include <Config.h>
#include <GameState.h>
#include <SlideSwitch.h>

void turnOffAllLEDs();

GameState state = makeState(0, 0);

static inline void tictactoe_setup() {
#if USE_SERIAL
  Serial.begin(9600);
#endif
  for (uint8_t i = 0; i < PIN_COUNT; i++) {
    pinMode(PINS[i], OUTPUT);
  }
  for (uint8_t i = 0; i < BUTTON_COL_COUNT; i++) {
    pinMode(BUTTONS_COLS[i], OUTPUT);
    digitalWrite(BUTTONS_COLS[i], LOW);
  }
  for (uint8_t i = 0; i < BUTTON_ROW_COUNT; i++) {
    pinMode(BUTTONS_ROWS[i], INPUT_PULLUP);
  }
  pinMode(SLIDE_LEFT, INPUT_PULLUP);
  pinMode(SLIDE_RIGHT, INPUT_PULLUP);
  turnOffAllLEDs();
}

#endif
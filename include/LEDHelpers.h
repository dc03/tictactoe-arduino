#pragma once

/* Copyright (C) 2023 Dhruv Chawla */
/* See LICENSE at project root for license details */

#ifndef LED_HELPERS_H
#define LED_HELPERS_H

#include <CellType.h>
#include <Config.h>

static inline void turnOffAllLEDs() {
  for (uint8_t i = 0; i < LED_ROW_COUNT; i++) {
    digitalWrite(ROWS[i], HIGH);
  }
  for (uint8_t i = LED_ROW_COUNT; i < PIN_COUNT; i++) {
    digitalWrite(PINS[i], LOW);
  }
}

static inline void turnOnLED(CellType type, uint8_t row, uint8_t col) {
  // turnOffAll();
  switch (type) {
    case CELL_X:
      digitalWrite(GREEN_COLS[col], HIGH);
      digitalWrite(ROWS[row], LOW);
      break;
    case CELL_O:
      digitalWrite(RED_COLS[col], HIGH);
      digitalWrite(ROWS[row], LOW);
      break;
  }
}

static inline void turnOffLED(CellType type, uint8_t row, uint8_t col) {
  // turnOffAll();
  switch (type) {
    case CELL_X:
      digitalWrite(GREEN_COLS[col], LOW);
      digitalWrite(ROWS[row], HIGH);
      break;
    case CELL_O:
      digitalWrite(RED_COLS[col], LOW);
      digitalWrite(ROWS[row], HIGH);
      break;
  }
}

#endif
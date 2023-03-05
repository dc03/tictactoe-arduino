#pragma once

/* Copyright (C) 2023 Dhruv Chawla */
/* See LICENSE at project root for license details */

#ifndef BUTTON_PRESS_H
#define BUTTON_PRESS_H

#include <Config.h>

/**
 * @brief Check if a button was pressed
 *
 * A non-zero return value indicates that a button was pressed
 *
 * @return uint8_t The pressed button
 */
static inline uint8_t pressedButton() {
  for (uint8_t i = 0; i < BUTTON_COL_COUNT; i++) {
    digitalWrite(BUTTONS_COLS[i], HIGH);
  }
  for (uint8_t i = 0; i < BUTTON_COL_COUNT; i++) {
    digitalWrite(BUTTONS_COLS[i], LOW);
    for (uint8_t j = 0; j < BUTTON_ROW_COUNT; j++) {
      if (digitalRead(BUTTONS_ROWS[j]) == LOW) {
        digitalWrite(BUTTONS_COLS[i], HIGH);
        return (i << 3) | (j << 1) | 1;
      }
    }
    digitalWrite(BUTTONS_COLS[i], HIGH);
  }
  return 0;
}

static inline uint8_t buttonRow(uint8_t button) {
  return (button >> 1) & 0b11;
}

static inline uint8_t buttonCol(uint8_t button) {
  return (button >> 3) & 0b11;
}

#endif
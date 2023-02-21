#pragma once

#ifndef DRAW_STATE_HELPERS_H
#define DRAW_STATE_HELPERS_H

#include <CellType.h>
#include <Config.h>
#include <GameState.h>
#include <LEDHelpers.h>

static inline void drawGameState(GameState state) {
  uint32_t mask = 1;
  for (uint8_t i = 0; i < 3; i++) {
    for (uint8_t j = 0; j < 3; j++) {
      if (state.crosses & mask) {
        turnOnLED(CELL_X, i, j);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_X, i, j);
      }
      if (state.noughts & mask) {
        turnOnLED(CELL_O, i, j);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_O, i, j);
      }
      mask <<= 1;
    }
  }
}

static inline void drawGameStateEquiDelay(GameState state) {
  uint32_t mask = 1;
  bool something_changed = false;
  for (uint8_t i = 0; i < 3; i++) {
    for (uint8_t j = 0; j < 3; j++) {
      something_changed = false;
      if (state.crosses & mask) {
        turnOnLED(CELL_X, i, j);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_X, i, j);
        something_changed = true;
      }
      if (state.noughts & mask) {
        turnOnLED(CELL_O, i, j);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_O, i, j);
        something_changed = true;
      }
      if (!something_changed) { DELAY_FUNCTION(DELAY); }
      mask <<= 1;
    }
  }
}

static inline void drawGameRow(GameState state, uint8_t row) {
  for (uint8_t j = 0; j < 3; j++) {
    uint16_t mask = 1 << ((row * 3) + j);
    if (state.crosses & mask) {
      turnOnLED(CELL_X, row, j);
      DELAY_FUNCTION(DELAY);
      turnOffLED(CELL_X, row, j);
    }
    if (state.noughts & mask) {
      turnOnLED(CELL_O, row, j);
      DELAY_FUNCTION(DELAY);
      turnOffLED(CELL_O, row, j);
    }
  }
}

static inline void drawGameCol(GameState state, uint8_t col) {
  for (uint8_t i = 0; i < 3; i++) {
    uint16_t mask = 1 << ((i * 3) + col);
    if (state.crosses & mask) {
      turnOnLED(CELL_X, i, col);
      DELAY_FUNCTION(DELAY);
      turnOffLED(CELL_X, i, col);
    }
    if (state.noughts & mask) {
      turnOnLED(CELL_O, i, col);
      DELAY_FUNCTION(DELAY);
      turnOffLED(CELL_O, i, col);
    }
  }
}

static inline void drawGameDiag(GameState state, bool ismain) {
  if (ismain) {
    for (uint8_t i = 0; i < 3; i++) {
      if (state.crosses & (1 << ((i * 3) + i))) {
        turnOnLED(CELL_X, i, i);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_X, i, i);
      }
      if (state.noughts & (1 << ((i * 3) + i))) {
        turnOnLED(CELL_O, i, i);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_O, i, i);
      }
    }
  } else {
    for (uint8_t i = 0; i < 3; i++) {
      if (state.crosses & (1 << ((i * 3) + (2 - i)))) {
        turnOnLED(CELL_X, i, 2 - i);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_X, i, 2 - i);
      }
      if (state.noughts & (1 << ((i * 3) + (2 - i)))) {
        turnOnLED(CELL_O, i, 2 - i);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_O, i, 2 - i);
      }
    }
  }
}

#endif
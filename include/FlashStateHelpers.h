#pragma once

/* Copyright (C) 2023 Dhruv Chawla */
/* See LICENSE at project root for license details */

#ifndef FLASH_STATE_HELPERS_H
#define FLASH_STATE_HELPERS_H

#include <CellType.h>
#include <Config.h>
#include <DrawStateHelpers.h>
#include <GameState.h>

static inline void flashGameRow(GameState state, uint8_t row) {
  for (uint8_t i = 0; i < TIMES_TO_RUN; i++) {
    drawGameRow(state, row);
  }
  delay(PAUSE_DELAY);
}

static inline void flashGameCol(GameState state, uint8_t col) {
  for (uint8_t i = 0; i < TIMES_TO_RUN; i++) {
    drawGameCol(state, col);
  }
  delay(PAUSE_DELAY);
}

static inline void flashGameDiag(GameState state, bool ismain) {
  for (uint8_t i = 0; i < TIMES_TO_RUN; i++) {
    drawGameDiag(state, ismain);
  }
  delay(PAUSE_DELAY);
}

static inline void flashGameState(GameState state) {
  for (uint8_t i = 0; i < TIMES_TO_RUN; i++) {
    drawGameState(state);
  }
  delay(PAUSE_DELAY);
}

#endif
#pragma once

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <CellType.h>
#include <Config.h>
#include <stdint.h>

struct GameState {
  uint32_t crosses : 9;
  uint32_t noughts : 9;
};

static inline GameState makeState(uint16_t crosses, uint16_t noughts) {
  return GameState{
      static_cast<uint32_t>(crosses & 0b111111111),
      static_cast<uint32_t>(noughts & 0b111111111),
  };
}

template <uint8_t rows, uint8_t cols>
static inline GameState stateFromArray(const uint8_t (&statearr)[rows][cols]) {
  GameState state{};
  for (uint8_t i = 0; i < rows; i++) {
    for (uint8_t j = 0; j < cols; j++) {
      if (statearr[i][j] & CELL_X) {
        state.crosses |= 1 << ((i * COL_COUNT) + j);
      }
      if (statearr[i][j] & CELL_O) {
        state.noughts |= 1 << ((i * COL_COUNT) + j);
      }
    }
  }
  return state;
}

#endif
#pragma once

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <CellType.h>
#include <Config.h>
#include <stdint.h>

enum PlayerState : uint8_t {
  PLAYER_X = 0,
  PLAYER_O = 1,
  UNKNOWN = 2,
};

enum GameWinPosition : uint8_t {
  NONE_ = 0,
  ROW_0 = 1,
  ROW_1 = 2,
  ROW_2 = 3,
  COL_0 = 4,
  COL_1 = 5,
  COL_2 = 6,
  DIA_1 = 7,
  DIA_2 = 8,
  DRAW_ = 9,
};

struct GameState {
  uint32_t crosses : 9;
  uint32_t noughts : 9;
  uint32_t player : 1;
};

static inline GameState makeState(uint16_t crosses, uint16_t noughts) {
  return GameState{
      static_cast<uint32_t>(crosses & 0b111111111),
      static_cast<uint32_t>(noughts & 0b111111111),
      PLAYER_X,
  };
}

template <uint8_t rows, uint8_t cols>
static inline GameState stateFromArray(const uint8_t (&statearr)[rows][cols]) {
  GameState state{};
  state.player = PLAYER_X;
  for (uint8_t i = 0; i < rows; i++) {
    for (uint8_t j = 0; j < cols; j++) {
      if (statearr[i][j] & CELL_X) { state.crosses |= 1 << ((i * cols) + j); }
      if (statearr[i][j] & CELL_O) { state.noughts |= 1 << ((i * cols) + j); }
    }
  }
  return state;
}

static inline GameState setCellWithPlayer(GameState state, uint8_t row, uint8_t col, PlayerState player) {
  uint32_t mask = 1 << (row * 3 + col);
  if (player == PLAYER_X) {
    state.crosses |= mask;
  } else if (player == PLAYER_O) {
    state.noughts |= mask;
  }
  return state;
}

static inline GameState unsetCellWithPlayer(GameState state, uint8_t row, uint8_t col, PlayerState player) {
  uint32_t mask = 1 << (row * 3 + col);
  if (player == PLAYER_X && (state.crosses & mask)) {
    state.crosses ^= mask;
  } else if (player == PLAYER_O && (state.noughts & mask)) {
    state.noughts ^= mask;
  }
  return state;
}

static inline bool isSetCell(GameState state, uint8_t row, uint8_t col) {
  uint32_t mask = 1 << (row * 3 + col);
  return ((state.crosses | state.noughts) & mask) != 0;
}

static inline bool isCellEmpty(GameState state, uint8_t row, uint8_t col) {
  uint32_t mask = 1 << (row * 3 + col);
  return !((state.crosses & mask) || (state.noughts & mask));
}

static inline PlayerState whoHasCell(GameState state, uint8_t row, uint8_t col) {
  uint32_t mask = 1 << (row * 3 + col);
  if (state.crosses & mask) {
    return PLAYER_X;
  } else if (state.noughts & mask) {
    return PLAYER_O;
  } else {
    return UNKNOWN;
  }
}

static inline GameState trySetCell(GameState state, uint8_t row, uint8_t col) {
  uint32_t mask = 1 << (row * 3 + col);
  if (state.player == PLAYER_X && !(state.noughts & mask)) {
    state.player = PLAYER_O * !(state.crosses & mask);
    state.crosses |= mask;
  } else if (state.player == PLAYER_O && !(state.crosses & mask)) {
    state.player = PLAYER_X * !(state.noughts & mask);
    state.noughts |= mask;
  }
  return state;
}

#define ELEM(s, i, j) ((((s) & (1 << ((i)*3 + (j)))) >> ((i)*3 + (j))) & 0b1)
#define ROW(matrix, i)                                                                                                 \
  (ELEM(matrix, (i), 0) != 0 && ELEM(matrix, (i), 0) == ELEM(matrix, (i), 1) &&                                        \
      ELEM(matrix, (i), 1) == ELEM(matrix, (i), 2))
#define COL(matrix, i)                                                                                                 \
  (ELEM(matrix, 0, (i)) != 0 && ELEM(matrix, 0, (i)) == ELEM(matrix, 1, (i)) &&                                        \
      ELEM(matrix, 1, (i)) == ELEM(matrix, 2, (i)))

static inline GameWinPosition tryDetectWinOf(uint16_t matrix) {
  if (ROW(matrix, 0)) { return ROW_0; }
  if (ROW(matrix, 1)) { return ROW_1; }
  if (ROW(matrix, 2)) { return ROW_2; }
  if (COL(matrix, 0)) { return COL_0; }
  if (COL(matrix, 1)) { return COL_1; }
  if (COL(matrix, 2)) { return COL_2; }
  if (ELEM(matrix, 0, 0) != 0 && ELEM(matrix, 0, 0) == ELEM(matrix, 1, 1) && ELEM(matrix, 1, 1) == ELEM(matrix, 2, 2)) {
    return DIA_1;
  }
  if (ELEM(matrix, 0, 2) != 0 && ELEM(matrix, 0, 2) == ELEM(matrix, 1, 1) && ELEM(matrix, 1, 1) == ELEM(matrix, 2, 0)) {
    return DIA_2;
  }

  return NONE_;
}

static inline GameWinPosition tryDetectWin(GameState state) {
  GameWinPosition pos = tryDetectWinOf(state.crosses);
  if (pos != NONE_) {
    return pos;
  } else if ((pos = tryDetectWinOf(state.noughts)) != NONE_) {
    return pos;
  } else if ((state.crosses | state.noughts) == 0b111111111) {
    return DRAW_;
  }
  return NONE_;
}

// https://stackoverflow.com/questions/109023/count-the-number-of-set-bits-in-a-32-bit-integer
static inline uint8_t numberOfSetBits(uint32_t i) {
  // Java: use int, and use >>> instead of >>. Or use Integer.bitCount()
  // C or C++: use uint32_t
  i = i - ((i >> 1) & 0x55555555);                // add pairs of bits
  i = (i & 0x33333333) + ((i >> 2) & 0x33333333); // quads
  i = (i + (i >> 4)) & 0x0F0F0F0F;                // groups of 8
  return (i * 0x01010101) >> 24;                  // horizontal sum of bytes
}

static inline uint8_t setPlacesX(GameState state) {
  return numberOfSetBits(state.crosses);
}

static inline uint8_t setPlacesO(GameState state) {
  return numberOfSetBits(state.noughts);
}

static inline uint8_t setPlaces(GameState state) {
  return numberOfSetBits(state.crosses) + numberOfSetBits(state.noughts);
}

#endif
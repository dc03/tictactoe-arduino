#pragma once

/* Copyright (C) 2023 Dhruv Chawla */
/* See LICENSE at project root for license details */

#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include <Config.h>
#include <GameState.h>
#include <PrintBoard.h>
#include <math.h>

[[noreturn]] static inline void hang() {
  while (true) {}
}

static inline void debugPrint(const char *msg) {
#if USE_SERIAL
  Serial.println(msg);
#endif
}

static inline GameState makeRandomMove(GameState state) {
  if (setPlaces(state) == 9) {
    debugPrint("Cannot make move when all places are set");
    hang();
  }

  bool set = false;
  while (!set) {
    uint32_t rnd = millis() % 9;
    uint8_t row = rnd / 3;
    uint8_t col = rnd % 3;
    if (!isSetCell(state, row, col)) {
      state = trySetCell(state, row, col);
      set = true;
      break;
    }
  }
  return state;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MiniMax algorithm
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_SCORE 50

static inline int8_t getScore(GameState state, uint8_t depth) {
  GameWinPosition pos = tryDetectWin(state);
  if (pos == NONE_) {
    return 0;
  } else if (state.player == PLAYER_O) {
    return depth - MAX_SCORE;
  } else if (state.player == PLAYER_X) {
    return MAX_SCORE - depth;
  } else {
    debugPrint("Unreachable location in getScore reached");
    hang();
    return -1;
  }
}

static inline int8_t minimax(GameState state, uint8_t depth, bool is_max) {
  int8_t score = getScore(state, depth);
  if (score != 0) { return score; }
  if (setPlaces(state) == 9) { return 0; }

  if (is_max) {
    int8_t best = -100;
    for (uint8_t i = 0; i < 3; i++) {
      for (uint8_t j = 0; j < 3; j++) {
        if (isCellEmpty(state, i, j)) {
          state = setCellWithPlayer(state, i, j, PLAYER_O);
          best = max(best, minimax(state, depth + 1, false));
          state = unsetCellWithPlayer(state, i, j, PLAYER_O);
#if !AI_EASY
          if (tryDetectWin(setCellWithPlayer(state, i, j, PLAYER_X))) { best += 10; }
#endif
        }
      }
    }
    return best;
  } else {
    int8_t best = 100;
    for (uint8_t i = 0; i < 3; i++) {
      for (uint8_t j = 0; j < 3; j++) {
        if (isCellEmpty(state, i, j)) {
          state = setCellWithPlayer(state, i, j, PLAYER_X);
          best = min(best, minimax(state, depth + 1, true));
#if !AI_EASY
          if (tryDetectWin(setCellWithPlayer(state, i, j, PLAYER_O))) { best -= 10; }
#endif
          state = unsetCellWithPlayer(state, i, j, PLAYER_X);
        }
      }
    }
    return best;
  }
}

struct BestMove {
  uint8_t row : 3;
  uint8_t col : 3;
};

static inline BestMove findBestMove(GameState state) {
  int8_t best = -100;
  BestMove best_move{0, 0};

#if !AI_EASY
  int8_t bestMin = 100;
#endif

#if USE_SERIAL && AI_DEBUG_PRINT
  Serial.println("\nMINIMAX RESULT:"
                 "\n--------------\n");
#endif

  for (uint8_t i = 0; i < 3; i++) {
    for (uint8_t j = 0; j < 3; j++) {
      if (isCellEmpty(state, i, j)) {
        state = setCellWithPlayer(state, i, j, PLAYER_O);
        int8_t val = minimax(state, 0, false);
        state = unsetCellWithPlayer(state, i, j, PLAYER_O);
        if (val > best) {
          best = val;
          best_move.row = i;
          best_move.col = j;
        }
#if !AI_EASY
        if (val < bestMin) {
          bestMin = val;
          if (tryDetectWin(setCellWithPlayer(state, i, j, PLAYER_O)) != NONE_) {
            best_move.row = i;
            best_move.col = j;
            goto after;
          }
        }
#endif

#if USE_SERIAL && AI_DEBUG_PRINT
        printBoard(trySetCell(state, i, j));
        Serial.print("Move: row: ");
        Serial.println(i);
        Serial.print("      col: ");
        Serial.println(j);
        Serial.print("Score: ");
        Serial.println(val);
#endif
      }
    }
  }

after:

#if USE_SERIAL && AI_DEBUG_PRINT
  Serial.print("\n---------\nBest move: row: ");
  Serial.print(best_move.row);
  Serial.print(" col: ");
  Serial.println(best_move.col);
#endif

  return best_move;
}

static inline GameState makeMiniMaxMove(GameState state) {
  BestMove move = findBestMove(state);
  return trySetCell(state, move.row, move.col);
}

static inline GameState makeAIMove(GameState state) {
  switch (setPlaces(state)) {
    case 0:
    default:
      debugPrint("Unreachable condition in makeAIMove reached");
      hang();
      break;
    case 1:
    case 2: return makeRandomMove(state);
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9: return makeMiniMaxMove(state);
  }
}

#endif
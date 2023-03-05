#pragma once

#ifndef LOOP_FUNCTION_H
#define LOOP_FUNCTION_H

#include <AIPlayer.h>
#include <ButtonPress.h>
#include <CellType.h>
#include <Config.h>
#include <DrawStateHelpers.h>
#include <FlashStateHelpers.h>
#include <GameState.h>
#include <LEDHelpers.h>
#include <SlideSwitch.h>

extern GameState state;

static inline void tictactoe_loop_stub() {}

static inline void detectWin() {
  GameWinPosition pos = tryDetectWin(state);
  // clang-format off
  switch (pos) {
    case ROW_0: while (true) {flashGameRow(state, 0);}
    case ROW_1: while (true) {flashGameRow(state, 1);}
    case ROW_2: while (true) {flashGameRow(state, 2);}
    case COL_0: while (true) {flashGameCol(state, 0);}
    case COL_1: while (true) {flashGameCol(state, 1);}
    case COL_2: while (true) {flashGameCol(state, 2);}
    case DIA_1: while (true) {flashGameDiag(state, true);}
    case DIA_2: while (true) {flashGameDiag(state, false);}
    case DRAW_: while (true) {flashGameState(state);}
    default:
      break;
  }
  // clang-format on
}

static inline uint8_t handlePlayerInput() {
  static uint8_t prev_button = 0;
  uint8_t button_pressed = pressedButton();
  if (button_pressed && button_pressed != prev_button) {
    uint8_t row = buttonRow(button_pressed);
    uint8_t col = buttonCol(button_pressed);
    if (!isCellEmpty(state, row, col)) { return 0; }
    state = trySetCell(state, row, col);
    prev_button = button_pressed;
    return button_pressed;
  } else {
    return 0;
  }
}

static inline void tictactoe_1P_mode() {
  drawGameState(state);
  uint8_t button_pressed = 0;
  if ((button_pressed = handlePlayerInput())) {
    detectWin();
    for (uint16_t i = 0; i < 50; i++) {
      drawGameStateEquiDelay(state);
      DELAY_FUNCTION(DELAY);
    }
    state = makeAIMove(state);
    detectWin();
  }
}

static inline void tictactoe_2P_mode() {
  drawGameState(state);
  if (handlePlayerInput()) { detectWin(); }
}

static void (*tictactoe_loop)() = tictactoe_loop_stub;

#endif
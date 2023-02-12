#pragma once

#ifndef LOOP_FUNCTION_H
#define LOOP_FUNCTION_H

#include <ButtonPress.h>
#include <CellType.h>
#include <Config.h>
#include <DrawStateHelpers.h>
#include <FlashStateHelpers.h>
#include <GameState.h>
#include <LEDHelpers.h>
#include <SlideSwitch.h>

extern GameState state;

static inline void tictactoe_loop() {
  drawGameState(state);
  uint8_t button_pressed = pressedButton();
  if (button_pressed) {
    uint8_t row = buttonRow(button_pressed);
    uint8_t col = buttonCol(button_pressed);
    state = trySetCell(state, row, col);
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
}

#endif
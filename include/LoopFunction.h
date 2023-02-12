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

// constexpr uint8_t statearr[][3] = {
//     {CELL_X | CELL_O, CELL_X | CELL_O, CELL_X | CELL_O},
//     {CELL_X | CELL_O, CELL_X | CELL_O, CELL_X | CELL_O},
//     {CELL_X | CELL_O, CELL_X | CELL_O, CELL_X | CELL_O},
// };

// GameState state = makeState(0b000111000, 0b010000101);
extern GameState state;
// GameState state = makeState(0, 0);

static inline void tictactoe_loop() {
  // for (uint8_t i = 0; i < ROW_COUNT; i++) {
  //   flashGameRow(state, i);
  // }
  // for (uint8_t i = 0; i < COL_COUNT; i++) {
  //   flashGameCol(state, i);
  // }
  // flashGameDiag(state, true);
  // flashGameDiag(state, false);
  // flashGameState(state);
  // flashGameState(state);
  // for (int i = 0; i < BUTTON_COUNT; i++) {
  //   uint8_t value = digitalRead(BUTTONS[i]);
  //   if (!value) {
  //     Serial.print(i);
  //     Serial.println(" was pressed");
  //   }
  // }
  // uint8_t button_pressed = pressedButton();
  // // while (!(button_pressed = pressedButton())) {}
  // // while (pressedButton() == button_pressed) {}
  // if (button_pressed) {
  //   uint8_t row = (button_pressed >> 1) & 0b11;
  //   uint8_t col = (button_pressed >> 3) & 0b11;
  //   if (slideSwitchPosition() == SLIDE_IS_LEFT) {
  //     turnOnLED(CELL_X, row, col);
  //     DELAY_FUNCTION(DELAY);
  //     turnOffLED(CELL_X, row, col);
  //   }
  //   if (slideSwitchPosition() == SLIDE_IS_RIGHT) {
  //     turnOnLED(CELL_O, row, col);
  //     DELAY_FUNCTION(DELAY);
  //     turnOffLED(CELL_O, row, col);
  //   }
  // }
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
#pragma once

#ifndef LOOP_FUNCTION_H
#define LOOP_FUNCTION_H

#include <CellType.h>
#include <Config.h>
#include <FlashStateHelpers.h>
#include <GameState.h>

constexpr uint8_t statearr[][3] = {
    {CELL_X | CELL_O, CELL_X | CELL_O, CELL_X | CELL_O},
    {CELL_X | CELL_O, CELL_X | CELL_O, CELL_X | CELL_O},
    {CELL_X | CELL_O, CELL_X | CELL_O, CELL_X | CELL_O},
};

// GameState state = makeState(0b000111000, 0b010000101);
GameState state = stateFromArray(statearr);

static inline void tictactoe_loop() {
  turnOffAll();
  for (uint8_t i = 0; i < ROW_COUNT; i++) {
    flashGameRow(state, i);
  }
  for (uint8_t i = 0; i < COL_COUNT; i++) {
    flashGameCol(state, i);
  }
  flashGameDiag(state, true);
  flashGameDiag(state, false);
  flashGameState(state);
  flashGameState(state);
  // for (int i = 0; i < BUTTON_COUNT; i++) {
  //   uint8_t value = digitalRead(BUTTONS[i]);
  //   if (!value) {
  //     Serial.print(i);
  //     Serial.println(" was pressed");
  //   }
  // }
}

#endif
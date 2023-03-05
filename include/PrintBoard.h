#pragma once

#ifndef PRINT_BOARD_H
#define PRINT_BOARD_H

#include <Config.h>
#include <GameState.h>

void printBoard(GameState state) {
#if USE_SERIAL
  Serial.println("\n");
  for (uint8_t i = 0; i < 3; i++) {
    for (uint8_t j = 0; j < 3; j++) {
      Serial.print(" ");
      switch (whoHasCell(state, i, j)) {
        case PLAYER_X: Serial.print("X "); break;
        case PLAYER_O: Serial.print("O "); break;
        default: Serial.print("  "); break;
      }
      if (j < 2) { Serial.print("|"); }
    }
    if (i < 2) { Serial.println("\n---|---|---"); }
  }
  Serial.println("\n");
#endif
}

#endif
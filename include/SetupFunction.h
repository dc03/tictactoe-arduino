#pragma once

#include "FlashStateHelpers.h"
#ifndef SETUP_FUNCTION_H
#define SETUP_FUNCTION_H

#include <Config.h>
#include <GameState.h>
#include <SlideSwitch.h>

void turnOffAllLEDs();

extern void (*tictactoe_loop)();
extern void tictactoe_1P_mode();
extern void tictactoe_2P_mode();

GameState state = makeState(0, 0);

static inline void flashIntro() {
  uint8_t intro_times_to_draw = 5;
  GameState intro = makeState(0, 0);

  for (uint8_t i = 0; i < 3; i++) {
    for (uint8_t j = 0; j < 3; j++) {
      intro = setCellWithPlayer(intro, i, j, PLAYER_X);
      for (uint8_t k = 0; k < intro_times_to_draw; k++) {
        drawGameStateEquiDelay(intro);
      }
    }

    for (uint8_t j = 3; j > 0; j--) {
      intro = setCellWithPlayer(intro, i, j - 1, PLAYER_O);
      for (uint8_t k = 0; k < intro_times_to_draw; k++) {
        drawGameStateEquiDelay(intro);
      }
    }
  }
  DELAY_FUNCTION(PAUSE_DELAY);
  for (uint8_t i = 0; i < 2; i++) {
    for (uint8_t j = 0; j < intro_times_to_draw + 5; j++) {
      drawGameStateEquiDelay(intro);
    }
    DELAY_FUNCTION(PAUSE_DELAY);
  }
}

static inline void tictactoe_setup() {
#if USE_SERIAL
  Serial.begin(9600);
#endif
  for (uint8_t i = 0; i < PIN_COUNT; i++) {
    pinMode(PINS[i], OUTPUT);
  }
  for (uint8_t i = 0; i < BUTTON_COL_COUNT; i++) {
    pinMode(BUTTONS_COLS[i], OUTPUT);
    digitalWrite(BUTTONS_COLS[i], LOW);
  }
  for (uint8_t i = 0; i < BUTTON_ROW_COUNT; i++) {
    pinMode(BUTTONS_ROWS[i], INPUT_PULLUP);
  }
  pinMode(SLIDE_LEFT, INPUT_PULLUP);
  pinMode(SLIDE_RIGHT, INPUT_PULLUP);
  turnOffAllLEDs();

  switch (slideSwitchPosition()) {
    case SLIDE_IS_LEFT: tictactoe_loop = tictactoe_2P_mode; break;
    case SLIDE_IS_RIGHT: tictactoe_loop = tictactoe_1P_mode; break;
  }

#if INTRO_SHOW
  flashIntro();
#endif
}

#endif
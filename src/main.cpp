#include <Arduino.h>

const int ROWS[] = {5, 6, 7};
const int GREEN_COLS[] = {13, 11, 9};
const int RED_COLS[] = {12, 10, 8};

#define ROW_COUNT 3
#define COL_COUNT 3

const int PINS[] = {5, 6, 7, 8, 9, 10, 11, 12, 13};

#define PIN_COUNT 9

#define DELAY 1
#define DELAY_FUNCTION delay

void setup() {
  for (int i = 0; i < PIN_COUNT; i++) {
    pinMode(PINS[i], OUTPUT);
  }
}

enum CellType {
  CELL_X = 0,
  CELL_O = 1,
};

static void turnOffAll() {
  for (int i = 0; i < ROW_COUNT; i++) {
    digitalWrite(ROWS[i], HIGH);
  }
  for (int i = ROW_COUNT; i < PIN_COUNT; i++) {
    digitalWrite(PINS[i], LOW);
  }
}

static void turnOnLED(CellType type, uint8_t row, uint8_t col) {
  turnOffAll();
  switch (type) {
    case CELL_X:
      digitalWrite(GREEN_COLS[col], HIGH);
      digitalWrite(ROWS[row], LOW);
      break;
    case CELL_O:
      digitalWrite(RED_COLS[col], HIGH);
      digitalWrite(ROWS[row], LOW);
      break;
  }
}

static void turnOffLED(CellType type, uint8_t row, uint8_t col) {
  turnOffAll();
  switch (type) {
    case CELL_X:
      digitalWrite(GREEN_COLS[col], LOW);
      digitalWrite(ROWS[row], HIGH);
      break;
    case CELL_O:
      digitalWrite(RED_COLS[col], LOW);
      digitalWrite(ROWS[row], HIGH);
      break;
  }
}

struct GameState {
  uint32_t crosses : 9;
  uint32_t noughts : 9;
};

GameState makeState(uint16_t crosses, uint16_t noughts) {
  return GameState {
    static_cast<uint32_t>(crosses & 0b111111111),
    static_cast<uint32_t>(noughts & 0b111111111),
  };
}

void drawGameState(GameState state) {
  uint32_t mask = 1 << 8;
  for (int i = 0; i < ROW_COUNT; i++) {
    for (int j = 0; j < COL_COUNT; j++) {
      if (state.crosses & mask) {
        turnOnLED(CELL_X, i, j);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_X, i, j);
        DELAY_FUNCTION(DELAY);
      } else if (state.noughts & mask) {
        turnOnLED(CELL_O, i, j);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_O, i, j);
        DELAY_FUNCTION(DELAY);
      }
      mask >>= 1;
    }
  }
}

GameState state = makeState(0b000111000, 0b010000101);

void loop() {
  // put your main code here, to run repeatedly:
  drawGameState(state);
}
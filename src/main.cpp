#include <Arduino.h>
#include <HardwareSerial.h>

const int ROWS[] = {5, 6, 7};
const int GREEN_COLS[] = {13, 11, 9};
const int RED_COLS[] = {12, 10, 8};

#define ROW_COUNT 3
#define COL_COUNT 3

const int PINS[] = {5, 6, 7, 8, 9, 10, 11, 12, 13};

#define PIN_COUNT 9

const int BUTTONS[] = {A0, A1, A2, A3, A4, A5};

#define BUTTON_COUNT 6

#define DELAY 1
#define DELAY_FUNCTION delay
#define TIMES_TO_RUN 20
#define PAUSE_DELAY 75

enum CellType {
  CELL_X = 0b01,
  CELL_O = 0b10,
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
  // turnOffAll();
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
  // turnOffAll();
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

GameState stateFromArray(int statearr[3][3]) {
  GameState state{};
  for (int i = 0; i < ROW_COUNT; i++) {
    for (int j = 0; j < COL_COUNT; j++) {
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

void drawGameState(GameState state) {
  uint32_t mask = 1;
  for (int i = 0; i < ROW_COUNT; i++) {
    for (int j = 0; j < COL_COUNT; j++) {
      if (state.crosses & mask) {
        turnOnLED(CELL_X, i, j);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_X, i, j);
      }
      if (state.noughts & mask) {
        turnOnLED(CELL_O, i, j);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_O, i, j);
      }
      mask <<= 1;
    }
  }
}

void drawGameRow(GameState state, uint8_t row) {
  for (int j = 0; j < COL_COUNT; j++) {
    uint16_t mask = 1 << ((row * COL_COUNT) + j);
    if (state.crosses & mask) {
      turnOnLED(CELL_X, row, j);
      DELAY_FUNCTION(DELAY);
      turnOffLED(CELL_X, row, j);
    }
    if (state.noughts & mask) {
      turnOnLED(CELL_O, row, j);
      DELAY_FUNCTION(DELAY);
      turnOffLED(CELL_O, row, j);
    }
  }
}

void drawGameCol(GameState state, uint8_t col) {
  for (int i = 0; i < ROW_COUNT; i++) {
    uint16_t mask = 1 << ((i * COL_COUNT) + col);
    if (state.crosses & mask) {
      turnOnLED(CELL_X, i, col);
      DELAY_FUNCTION(DELAY);
      turnOffLED(CELL_X, i, col);
    }
    if (state.noughts & mask) {
      turnOnLED(CELL_O, i, col);
      DELAY_FUNCTION(DELAY);
      turnOffLED(CELL_O, i, col);
    }
  }
}

void drawGameDiag(GameState state, bool ismain) {
  if (ismain) {
    for (int i = 0; i < 3; i++) {
      if (state.crosses & (1 << ((i * COL_COUNT) + i))) {
        turnOnLED(CELL_X, i, i);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_X, i, i);
      }
      if (state.noughts & (1 << ((i * COL_COUNT) + i))) {
        turnOnLED(CELL_O, i, i);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_O, i, i);
      }
    }
  } else {
    for (int i = 0; i < 3; i++) {
      if (state.crosses & (1 << ((i * COL_COUNT) + (2 - i)))) {
        turnOnLED(CELL_X, i, 2 - i);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_X, i, 2 - i);
      }
      if (state.noughts & (1 << ((i * COL_COUNT) + (2 - i)))) {
        turnOnLED(CELL_O, i, 2 - i);
        DELAY_FUNCTION(DELAY);
        turnOffLED(CELL_O, i, 2 - i);
      }
    }
  }
}

void flashGameRow(GameState state, uint8_t row) {
  for (int i = 0; i < TIMES_TO_RUN; i++) {
    drawGameRow(state, row);
  }
  delay(PAUSE_DELAY);
}

void flashGameCol(GameState state, uint8_t col) {
  for (int i = 0; i < TIMES_TO_RUN; i++) {
    drawGameCol(state, col);
  }
  delay(PAUSE_DELAY);
}

void flashGameDiag(GameState state, bool ismain) {
  for (int i = 0; i < TIMES_TO_RUN; i++) {
    drawGameDiag(state, ismain);
  }
  delay(PAUSE_DELAY);
}

void flashGameState(GameState state) {
  for (int i = 0; i < TIMES_TO_RUN; i++) {
    drawGameState(state);
  }
  delay(PAUSE_DELAY);
}

int statearr[][3] = {
  {CELL_X|CELL_O, CELL_X|CELL_O, CELL_X|CELL_O},
  {CELL_X|CELL_O, CELL_X|CELL_O, CELL_X|CELL_O},
  {CELL_X|CELL_O, CELL_X|CELL_O, CELL_X|CELL_O},
};

//GameState state = makeState(0b000111000, 0b010000101);
GameState state = stateFromArray(statearr);

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < PIN_COUNT; i++) {
    pinMode(PINS[i], OUTPUT);
  }
  for (int i = 0; i < BUTTON_COUNT; i++) {
    pinMode(BUTTONS[i], INPUT_PULLUP);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  turnOffAll();
  for (int i = 0; i < ROW_COUNT; i++) {
    flashGameRow(state, i);
  }
  for (int i = 0; i < COL_COUNT; i++) {
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
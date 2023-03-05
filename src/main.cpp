// #define USE_SERIAL_CONSOLE
// #define NO_SHOW_INTRO
// #define AI_DEBUG_MODE
// #define AI_EASY_MODE

#include <LoopFunction.h>
#include <SetupFunction.h>

void setup() {
  tictactoe_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  tictactoe_loop();
}
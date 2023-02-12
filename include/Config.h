#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#ifdef USE_SERIAL_CONSOLE
#define USE_SERIAL   1
#define IF_SERIAL(x) x
#else
#define USE_SERIAL 0
#define IF_SERIAL(x)
#endif

#if USE_SERIAL == 0
#define HardwareSerial_h
#endif

#include <Arduino.h>
#include <stdint.h>

constexpr static uint8_t ROWS[] = {5, 6, 7};
constexpr static uint8_t GREEN_COLS[] = {13, 11, 9};
constexpr static uint8_t RED_COLS[] = {12, 10, 8};
constexpr static uint8_t PINS[] = {5, 6, 7, 8, 9, 10, 11, 12, 13};

constexpr static uint8_t BUTTONS_COLS[] = {A0, A1, A2};
constexpr static uint8_t BUTTONS_ROWS[] = {A3, A4, A5};

#define SLIDE_LEFT     4
#define SLIDE_RIGHT    3
#define SLIDE_IS_LEFT  0
#define SLIDE_IS_RIGHT 1

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

static_assert(ARRAY_SIZE(GREEN_COLS) == ARRAY_SIZE(RED_COLS), "Expect green and red to be of same size");

#define ROW_COUNT        ARRAY_SIZE(ROWS)
#define COL_COUNT        ARRAY_SIZE(GREEN_COLS)
#define PIN_COUNT        ARRAY_SIZE(PINS)
#define BUTTON_ROW_COUNT ARRAY_SIZE(BUTTONS_ROWS)
#define BUTTON_COL_COUNT ARRAY_SIZE(BUTTONS_COLS)

#define DELAY          1
#define DELAY_FUNCTION delay
#define TIMES_TO_RUN   50
#define PAUSE_DELAY    200

static_assert(TIMES_TO_RUN < static_cast<uint8_t>(-1), "TIMES_TO_RUN should not exceed maximum uint8_t value");

#endif
#pragma once

#ifndef SLIDE_SWITCH_H
#define SLIDE_SWITCH_H

#include <Config.h>

/**
 * @brief Returns the slide switch position
 *
 * 0 = left
 * 1 = right
 *
 * @return uint8_t The slide switch position
 */
static inline uint8_t slideSwitchPosition() {
  if (digitalRead(SLIDE_LEFT) == LOW) { return SLIDE_IS_LEFT; }
  if (digitalRead(SLIDE_RIGHT) == LOW) { return SLIDE_IS_RIGHT; }
  return static_cast<uint8_t>(-1);
}

#endif
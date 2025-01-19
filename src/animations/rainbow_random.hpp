#ifndef RAINBOW_RANDOM_HPP
#define RAINBOW_RANDOM_HPP

#include <FastLED.h>
#include "pins.hpp"

namespace rainbow_random {

void init();
void animation(CRGB* leds);

}

#endif // !RAINBOW_RANDOM_HPP
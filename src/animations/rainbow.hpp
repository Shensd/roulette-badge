#ifndef RAINBOW_HPP
#define RAINBOW_HPP

#include <FastLED.h>
#include "pins.hpp"
#include "util.hpp"

namespace rainbow {

void init();
void animation(CRGB* leds);

}

#endif // !RAINBOW_HPP
#ifndef RAINBOW_SINGLE_HPP
#define RAINBOW_SINGLE_HPP

#include <FastLED.h>
#include "pins.hpp"

namespace rainbow_single {

void init();
void animation(CRGB* leds);

}


#endif // !RAINBOW_SINGLE_HPP
#ifndef RAINBOW_WHEEL_HPP
#define RAINBOW_WHEEL_HPP

#include <FastLED.h>
#include "pins.hpp"
#include "util.hpp"

namespace rainbow_wheel {

void init();
void animation(CRGB* leds);

}


#endif // !RAINBOW_WHEEL_HPP
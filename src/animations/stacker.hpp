#ifndef STACKER_HPP
#define STACKER_HPP

#include <FastLED.h>
#include "pins.hpp"

namespace stacker {

void init();
void animation(CRGB* leds);

}


#endif // !STACKER_HPP
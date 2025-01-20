#ifndef MORSE_HPP
#define MORSE_HPP

#include <FastLED.h>
#include "pins.hpp"
#include "util.hpp"

namespace morse {

void init();
void animation(CRGB* leds);

}


#endif // !MORSE_HPP
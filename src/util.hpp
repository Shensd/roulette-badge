#ifndef UTIL_HPP
#define UTIL_HPP

#include <FastLED.h>

namespace util {

const uint32_t COLOR_MAX = 255 * 7;
CRGB index_to_color(uint32_t index);

}

#endif // !UTIL_HPP
#include "rainbow.hpp"

namespace rainbow {

uint32_t color_index = 0;

void init() {

}

void animation(CRGB* leds) {
    FastLED.clearData();

    CRGB color = util::index_to_color(color_index);
    FastLED.showColor(color);

    color_index += 4;
    if(color_index >= util::COLOR_MAX) color_index = 255;

    delay(10);
}

}
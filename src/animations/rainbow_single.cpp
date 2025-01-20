#include "rainbow_single.hpp"

namespace rainbow_single {

uint32_t color_index = 255;
uint8_t selected_led = 0;

void init() {

}

void animation(CRGB* leds) {
    FastLED.clearData();

    leds[selected_led] = util::index_to_color(color_index);

    FastLED.show();

    selected_led++;
    if(selected_led >= NUM_LEDS) selected_led = 0;

    color_index += 6;
    if(color_index >= util::COLOR_MAX) color_index = 255;

    delay(20);
}

}
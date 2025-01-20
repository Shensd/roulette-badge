#include "rainbow_wheel.hpp"

namespace rainbow_wheel {

unsigned long millis_last_tick = 0;

void init() {
    millis_last_tick = millis();
}

uint8_t color_step = (255 * 6) / NUM_LEDS;

uint8_t rotation_offset = 0;

void animation(CRGB* leds) {    
    if(millis() < millis_last_tick + 30) return;

    millis_last_tick = millis();

    FastLED.clearData();
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = util::index_to_color(((((i + rotation_offset) % NUM_LEDS) + 1) * color_step) + 255);
    }
    FastLED.show();

    rotation_offset = (rotation_offset + 1) % NUM_LEDS;
}

}

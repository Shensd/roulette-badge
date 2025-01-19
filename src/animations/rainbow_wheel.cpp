#include "rainbow_wheel.hpp"

namespace rainbow_wheel {

// leds spawn in at the 00 marker and fall down the sides to the 0 marker, 
// I'm thinking that it stacks up in one color then when the whole ring is that 
// color a different color starts stacking up on top of it 

unsigned long millis_last_tick = 0;

void init() {
    millis_last_tick = millis();
}

CRGB index_to_color(uint32_t index) {
    /*
    transition_to(leds, 255, 0, 0, delay_ms);
    transition_to(leds, 255, 255, 0, delay_ms);
    transition_to(leds, 0, 255, 0, delay_ms);
    transition_to(leds, 0, 255, 255, delay_ms);
    transition_to(leds, 0, 0, 255, delay_ms);
    transition_to(leds, 255, 0, 255, delay_ms);
    */

    if (index < 255) {
        return CRGB((uint8_t)index, (uint8_t)0, (uint8_t)0);  // go to (255, 0, 0)
    }
    if (index >= 255 && index < 255 * 2) {
        return CRGB((uint8_t)255, (uint8_t)index - (255 * 1), (uint8_t)0);  // go to (255, 255, 0)
    }
    if (index >= 255 * 2 && index < 255 * 3) {
        return CRGB((uint8_t)255 - (index - (255 * 2)), (uint8_t)255, (uint8_t)0);  // go to (0, 255, 0)
    }
    if (index >= 255 * 3 && index < 255 * 4) {
        return CRGB((uint8_t)0, (uint8_t)255, (uint8_t)index - (255 * 3));  // go to (0, 255, 255)
    }
    if (index >= 255 * 4 && index < 255 * 5) {
        return CRGB((uint8_t)0, (uint8_t)255 - (index - (255 * 4)), (uint8_t)255);  // go to (0, 0, 255)
    }
    if (index >= 255 * 5 && index < 255 * 6) {
        return CRGB((uint8_t)index - (255 * 5), (uint8_t)0, (uint8_t)255);  // go to (255, 0, 255)
    }
    if (index >= 255 * 6 && index < 255 * 7) {
        return CRGB((uint8_t)255, (uint8_t)0, (uint8_t)255 - (index - (255 * 6)));  // go to (255, 0, 0)
    }
}

uint32_t COLOR_MAX = 255 * 7;
uint8_t color_step = COLOR_MAX / NUM_LEDS;

uint8_t rotation_offset = 0;

void animation(CRGB* leds) {    
    if(millis() < millis_last_tick + 30) return;

    millis_last_tick = millis();

    FastLED.clearData();
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = index_to_color(((((i + rotation_offset) % NUM_LEDS) + 1) * color_step));
    }
    FastLED.show();

    rotation_offset = (rotation_offset + 1) % NUM_LEDS;
}

}
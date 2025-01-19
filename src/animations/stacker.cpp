#include "stacker.hpp"

namespace stacker {

uint32_t COLOR_MAX = 255 * 7;
CRGB index_to_color(uint32_t index) {
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

// we're just going to fill half and mirror it because that's easier
// uint8_t stack_buffer[NUM_LEDS / 2] = {0};
uint8_t stack_size = 0;
uint8_t glider_position = 0;
unsigned long millis_last_tick = 0;

void init() {}

uint32_t previous_color_index = 255;
uint32_t current_color_index = 510;

void animation(CRGB* leds) {
    if(millis() < millis_last_tick + 20) return;
    
    millis_last_tick = millis();

    glider_position++;
    if(stack_size == (NUM_LEDS / 2)) {
        stack_size = 0;
        glider_position = 0;
        uint32_t temp = previous_color_index;
        previous_color_index = current_color_index;
        current_color_index = (temp + (500) + random(255)) % COLOR_MAX;
    }
    if(glider_position == (NUM_LEDS / 2) - stack_size) {
        stack_size++;
        glider_position = 0;
    }

    FastLED.clearData();
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = index_to_color(previous_color_index);
    }
    for(int i = 0; i < stack_size; i++) {
        leds[i] = index_to_color(current_color_index);
        leds[(NUM_LEDS - 1) - i] = index_to_color(current_color_index);
    }
    leds[(NUM_LEDS / 2) + glider_position] = index_to_color(current_color_index);
    leds[(NUM_LEDS / 2) - glider_position] = index_to_color(current_color_index);
    FastLED.show();
}

}

#include "stacker.hpp"

namespace stacker {

uint8_t stack_size = 0;
uint8_t glider_position = 0;
unsigned long millis_last_tick = 0;

void init() {}

CRGB available_colors[] = {
    CRGB::Black,
    CRGB::Red,
};

uint32_t previous_color_index = 0;
uint32_t current_color_index = 1;

void animation(CRGB* leds) {
    if(millis() < millis_last_tick + 20) return;
    
    millis_last_tick = millis();

    glider_position++;
    if(stack_size == (NUM_LEDS / 2)) {
        stack_size = 0;
        glider_position = 0;
        previous_color_index = (previous_color_index + 1) % (sizeof(available_colors) / sizeof(available_colors[0]));
        current_color_index = (current_color_index + 1) % (sizeof(available_colors) / sizeof(available_colors[0]));
    }
    if(glider_position == (NUM_LEDS / 2) - stack_size) {
        stack_size++;
        glider_position = 0;
    }

    FastLED.clearData();
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = available_colors[previous_color_index];
    }
    for(int i = 0; i < stack_size; i++) {
        leds[i] = available_colors[current_color_index];
        leds[(NUM_LEDS - 1) - i] = available_colors[current_color_index];
    }
    leds[(NUM_LEDS / 2) + glider_position] = available_colors[current_color_index];
    leds[(NUM_LEDS / 2) - glider_position] = available_colors[current_color_index];
    FastLED.show();
}

}

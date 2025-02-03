#include "morse.hpp"

namespace morse {

// "NEVER STOP GAMBLING"
uint8_t morse_converted[] = {1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, };
uint32_t color_index = 255;

unsigned long millis_last_tick = 0;
unsigned long millis_tick_time = 100;

void init() {
    millis_last_tick = millis();
}

// magic numbers
uint8_t display_size = 25;
uint8_t display_offset = 7; 
uint32_t morse_offset = 0;

void animation(CRGB* leds) {
    if(millis() < millis_last_tick + millis_tick_time) return;

    millis_last_tick = millis();

    // I want it to have the display be full black for [display_size] before and after the morse

    CRGB morse_color = util::index_to_color(color_index);
    CRGB backdrop_color = CRGB::White;

    bool invert = false;

    FastLED.clearData();
    int32_t converted_offset = morse_offset - display_size;
    for(int i = 0; i < display_size; i++) {
        if(i + converted_offset < 0) {
            leds[NUM_LEDS - (i + display_offset)] = 0;
        } else if(i + converted_offset >= (sizeof(morse_converted) / sizeof(morse_converted[0]))) {
            leds[NUM_LEDS - (i + display_offset)] = 0;
        } else {
            leds[NUM_LEDS - (i + display_offset)] = morse_converted[i + converted_offset] == 1 ? morse_color : 0;
        }
    }

    leds[display_offset - 1] = backdrop_color;
    leds[NUM_LEDS - (display_offset - 1)] = backdrop_color;
    FastLED.show();

    morse_offset = (morse_offset + 1) % ((sizeof(morse_converted) / sizeof(morse_converted[0])) + display_size);
    color_index += 6;
    if(color_index >= util::COLOR_MAX) color_index = 255;
}

}
#include "morse.hpp"

namespace morse {

// "NEVER STOP GAMBLING"
uint8_t morse_converted[] = {1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, };

uint32_t COLOR_MAX = 255 * 7;
uint32_t color_index = 255;

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

unsigned long millis_last_tick = 0;
unsigned long millis_tick_time = 100;

void init() {
    millis_last_tick = millis();
}

// magic numbers
uint8_t display_size = 25;
uint8_t display_offset = 7; 
uint32_t morse_offset = 0;

// CRGB morse_color = CRGB::MediumVioletRed;
// CRGB backdrop_color = CRGB::White;

void animation(CRGB* leds) {
    if(millis() < millis_last_tick + millis_tick_time) return;

    millis_last_tick = millis();

    // I want it to have the display be full black for [display_size] before and after the morse

    CRGB morse_color = index_to_color(color_index);
    CRGB backdrop_color = CRGB::White;

    bool invert = false;

    FastLED.clearData();
    int32_t converted_offset = morse_offset - display_size;
    for(int i = 0; i < display_size; i++) {
        // if(i + morse_offset >= (sizeof(morse_converted) / sizeof(morse_converted[0]))) {
        //     leds[i + 7] = 0;
        // } else {
        //     leds[i + 7] = morse_converted[i + morse_offset] == 1 ? CRGB::Red : 0;
        // }
        if(i + converted_offset < 0) {
            leds[NUM_LEDS - (i + display_offset)] = 0;
        } else if(i + converted_offset >= (sizeof(morse_converted) / sizeof(morse_converted[0]))) {
            leds[NUM_LEDS - (i + display_offset)] = 0;
        } else {
            leds[NUM_LEDS - (i + display_offset)] = morse_converted[i + converted_offset] == 1 ? morse_color : 0;
        }
    }
    // for(int i = 0; i < display_offset; i++) {
    //     leds[i] = backdrop_color;
    //     leds[NUM_LEDS - i] = backdrop_color;
    // }
    leds[display_offset - 1] = backdrop_color;
    leds[NUM_LEDS - (display_offset - 1)] = backdrop_color;
    FastLED.show();

    morse_offset = (morse_offset + 1) % ((sizeof(morse_converted) / sizeof(morse_converted[0])) + display_size);
    color_index += 6;
    if(color_index >= COLOR_MAX) color_index = 255;
}

}
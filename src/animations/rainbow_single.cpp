#include "rainbow_single.hpp"

namespace rainbow_single {

uint32_t COLOR_MAX = 255 * 7;
uint32_t color_index = 255;
uint8_t selected_led = 0;

void init() {

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

void animation(CRGB* leds) {
    FastLED.clearData();

    leds[selected_led] = index_to_color(color_index);

    FastLED.show();

    selected_led++;
    if(selected_led >= NUM_LEDS) selected_led = 0;

    color_index += 6;
    if(color_index >= COLOR_MAX) color_index = 255;

    delay(20);
}

}
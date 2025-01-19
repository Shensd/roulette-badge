#include "rainbow_random.hpp"


namespace rainbow_random {

uint32_t MAX_STATE = 255 * 7;
uint32_t states[NUM_LEDS];


void init() {
    for(int i = 0; i < NUM_LEDS; i++) {
        states[i] = random(MAX_STATE);
    }
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

void run_transitions(CRGB* leds) {
    FastLED.clearData();
    uint8_t step = 6;
    for(int i = 0; i < NUM_LEDS; i+=2) {
        leds[i] = index_to_color(states[i]);
        states[i] += step;
        if(states[i] >= MAX_STATE) states[i] = 255;
    }
    FastLED.show();
}

void animation(CRGB* leds) {
    run_transitions(leds);
    delay(10);
}

}
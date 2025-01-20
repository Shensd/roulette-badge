#include "rainbow_random.hpp"


namespace rainbow_random {

uint32_t MAX_STATE = 255 * 7;
uint32_t states[NUM_LEDS];

void init() {
    for(int i = 0; i < NUM_LEDS; i++) {
        states[i] = random(MAX_STATE);
    }
}

void run_transitions(CRGB* leds) {
    FastLED.clearData();
    uint8_t step = 6;
    for(int i = 0; i < NUM_LEDS; i+=2) {
        leds[i] = util::index_to_color(states[i]);
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
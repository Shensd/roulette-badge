#include "sweeping_slice.hpp"

namespace sweeping_slice {

CRGB slider_dots[6] = {0};

uint32_t current_color = 255;
uint32_t next_color = 512;

void init() {
    for(int i = 0; i < sizeof(slider_dots) / sizeof(slider_dots[0]); i++) {
        slider_dots[i] = current_color;
    }
}

int8_t slice_position = 0;
int8_t slice_virtual_position = 0;
int8_t gravity = 1;
int8_t direction = 1;
uint32_t millis_last_tick = 0;

uint8_t slice_length = 7;

uint8_t bounce_min = 0;
uint8_t bounce_max = NUM_LEDS * 2;

void animation(CRGB* leds) {
    if(millis() < millis_last_tick + 40) return;
    millis_last_tick = millis();

    slice_position = (slice_position + (1 * direction)) % NUM_LEDS;
    if(slice_position < 0) slice_position = NUM_LEDS - 1;

    if(slice_position == 0) {
        current_color = next_color;
        next_color = random(6 * 255) + 255;
    }

    FastLED.clearData();
    leds[slice_position] = CRGB::White;
    for(int i = 1; i < (slice_length - 1); i++) {
        int dot_pos = (slice_position + i) % NUM_LEDS;
        if(dot_pos == 0) {
            slider_dots[i - 1] = util::index_to_color(next_color);
        }

        leds[dot_pos] = slider_dots[i - 1];
    }
    leds[(slice_position + (slice_length - 1)) % NUM_LEDS] = CRGB::White;
    FastLED.show();
}

}
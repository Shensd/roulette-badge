#include "roulette.hpp"

void run_roulette() {
    int8_t direction = random(2) == 1 ? 1 : -1;
    int8_t ball_index = random(38);

    CRGB* leds = FastLED.leds();

    FastLED.clearData();
    leds[ball_index] = CRGB::White;
    FastLED.show();

    delay(750);
    // delay(100);

    int32_t delay_time = 10;
    int32_t initial_spin = 38 + random(10);
    int32_t middle_spins = (38 * 2) + random((38 * 2));

    // very fast right out the gate
    for(int i = 0; i < initial_spin; i++) {
        ball_index = (ball_index + direction) % 38;
        if(ball_index < 0) ball_index = NUM_LEDS - 1;
        FastLED.clearData();
        leds[ball_index] = CRGB::White;
        FastLED.show();

        delay(delay_time);
    }

    delay_time += 2;

    // start slowing down for a few spins
    for(int i = 0; i < middle_spins; i++) {
        ball_index = (ball_index + direction) % 38;
        if(ball_index < 0) ball_index = NUM_LEDS - 1;
        FastLED.clearData();
        leds[ball_index] = CRGB::White;
        FastLED.show();

        delay(delay_time);

        if(i % 38 == 0) {
            delay_time += 1;
        }
    }

    // speed drop off
    uint32_t i = 0;
    while(true) {
        i++;

        if(i % 10) delay_time++;

        if(delay_time > 60) {
            if(random(20) == 0) break;
        }

        if(delay_time > 100) break;

        ball_index = (ball_index + direction) % 38;
        if(ball_index < 0) ball_index = NUM_LEDS - 1;
        FastLED.clearData();
        leds[ball_index] = CRGB::White;
        FastLED.show();
        
        delay(delay_time);
    }

    uint32_t final_spin = random(10);

    // go in for a final few steps
    for(int i = 0; i < final_spin; i++) {
        ball_index = (ball_index + direction) % 38;
        if(ball_index < 0) ball_index = NUM_LEDS - 1;
        FastLED.clearData();
        leds[ball_index] = CRGB::White;
        FastLED.show();

        delay_time += 10;
        delay(delay_time);
    }

    // winning number decided
    for(int i = 0; i < 3; i++) {
        FastLED.clearData();
        leds[ball_index] = CRGB::White;
        FastLED.show();
        delay(300);
        FastLED.clearData();
        FastLED.show();
        delay(300);
    }

    FastLED.clearData();
    leds[ball_index] = CRGB::White;
    // times_pulled[ball_index]++;

    // if(times_pulled[ball_index] <= 5) {
    //     leds[ball_index] = CRGB::Green;
    // }if(times_pulled[ball_index] > 5 && times_pulled[ball_index] <= 10) {
    //     leds[ball_index] = CRGB::Blue;
    // }if(times_pulled[ball_index] > 10) {
    //     leds[ball_index] = CRGB::Red;
    // }
    FastLED.show();

    delay(2000);
}
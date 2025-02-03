#include "quicktime.hpp"

namespace quicktime {

CRGB* leds;


/**
 * maybe make it like stacker? you have 5 leds blinking and a cursor that is 5 leds long, if you match
 * all 5 leds then you get 5 leds added to your stack, but if you only overlap 4 you only get 4
 * leds added to your stack, and then your cursor going forward is only 4 leds wide
 * 
 * maybe there is a hard cutoff where it gets force shorted to 3 then 2 then 1 or whatever
 */


const uint32_t STARTING_TICK_LEN = 100;
const uint32_t TICK_LEN_STEP = 2;
uint32_t tick_length = STARTING_TICK_LEN; 
int8_t cursor_position = 1;
int8_t target_position = 1;

void game_state_start() {
    FastLED.clearData();
    leds[0] = CRGB::Red;
    FastLED.show();

    delay(500);

    FastLED.clearData();
    leds[0] = CRGB::Red;
    leds[cursor_position] = CRGB::Blue;
    FastLED.show();

    delay(500);

    cursor_position++;
}

unsigned long millis_last_tick = 0;

void game_state_win() {
    for(int i = 0; i < 3; i++) {
        FastLED.clearData();
        FastLED.showColor(CRGB::Red);

        delay(500);

        FastLED.clearData();
        FastLED.show();

        delay(500);
    }

    // then it should display something in morse code 
}

void game_state_lost() {
    FastLED.clearData();
    for(int i = 0; i < target_position; i++) {
        leds[i] = CRGB::Red;
    }
    leds[cursor_position] = CRGB::White;
    FastLED.show();

    delay(500);

    FastLED.clearData();
    leds[cursor_position] = CRGB::White;
    FastLED.show();

    delay(500);

    FastLED.clearData();
    FastLED.show();

    delay(500);

    tick_length = STARTING_TICK_LEN;
    cursor_position = 1;
    target_position = 1;    

    game_state_start();
}

uint8_t position_led_state = 0;

void game_state_loop() {

    if(digitalRead(SPIN_BUTTON_PIN) == LOW) {
        if(cursor_position == target_position) {
            // successfully stacked
            target_position++;

            tick_length -= TICK_LEN_STEP;

            cursor_position = (target_position + 1) % NUM_LEDS;

            // game tick length should also be randomized here

            if(target_position == NUM_LEDS + 1) { // win state
                game_state_win();
            } else {
                FastLED.clearData();
                for(int i = 0; i < target_position; i++) {
                    leds[i] = CRGB::Red;
                }
                FastLED.show();

                while(digitalRead(SPIN_BUTTON_PIN) == LOW) {}

                delay(500);

                FastLED.clearData();
                for(int i = 0; i < target_position; i++) {
                    leds[i] = CRGB::Red;
                }
                leds[cursor_position - 1] = CRGB::Blue;
                FastLED.show();

                delay(tick_length);
            }
        } else {
            game_state_lost();
        }
    }

    if(millis() < millis_last_tick + tick_length) return;
    millis_last_tick = millis();

    FastLED.clearData();
    for(int i = 0; i < target_position; i++) {
        leds[i] = CRGB::Red;
    }
    if(position_led_state < 4) {
        leds[target_position] = CRGB::DarkBlue;
    }
    position_led_state = (position_led_state + 1) % 8;
    // i like the idea of it being purple when it overlaps but it makes it kinda hard to read
    // leds[cursor_position] = (cursor_position < target_position) ? CRGB::DarkMagenta : CRGB::Blue;
    leds[cursor_position] =  CRGB::Blue;
    
    FastLED.show();

    cursor_position = (cursor_position + 1) % NUM_LEDS;
}

void run_game(CRGB* l) {
    leds = l;

    game_state_start();

    while(true) {
        game_state_loop();
    }
}

}
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
const uint32_t TICK_LEN_STEP = 10;
uint32_t tick_length = STARTING_TICK_LEN; 
int8_t cursor_position = 0;
int8_t cursor_size = 5;
int8_t target_position = 14;
int8_t target_size = 5;
int8_t turn_number = 0;

// 38 total leds
// turn 1: 5 (5)
// turn 2: 5 (10)
// turn 3: 5 (15)
// turn 4: 4 (19)
// turn 5: 4 (23)
// turn 6: 4 (27)
// turn 7: 3 (30)
// turn 8: 3 (33)
// turn 9: 2 (35)
// turn 10: 1 (36)
// turn 11: 1 (37)
// turn 12: 1 (38)

const uint8_t target_size_steps[] = {
    5, 5, 5, 4, 4, 4, 3, 3, 2, 1, 1, 1
};
const uint8_t tick_len_steps[] = {
    100, 80, 70, 70, 70, 65, 65, 50, 50, 50, 40, 40
};

// bool target_led_state = true;
uint8_t target_led_max_brightness = 255;
int8_t target_led_brightness_direction = -1;
uint8_t target_led_brightness = 255;

void game_state_start() {
    // FastLED.clearData();
    // leds[0] = CRGB::Red;
    // FastLED.show();

    // delay(500);

    // FastLED.clearData();
    // leds[0] = CRGB::Red;
    // leds[cursor_position] = CRGB::Blue;
    // FastLED.show();

    // delay(500);

    // cursor_position++;

    target_position = 0;
    target_size = 5;
    cursor_position = target_position + target_size;
    cursor_size = 5;
    turn_number = 0;
    tick_length = STARTING_TICK_LEN;
    // target_led_state = true;
    target_led_brightness = target_led_max_brightness;
    target_led_brightness_direction = -1;
    // target_led_max_brightness = FastLED.getBrightness();
}

unsigned long millis_last_tick = 0;
unsigned long millis_last_target_blink_tick = 0;

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
    // FastLED.clearData();
    // for(int i = 0; i < target_position; i++) {
    //     leds[i] = CRGB::Red;
    // }
    // leds[cursor_position] = CRGB::White;
    // FastLED.show();

    // delay(500);

    // FastLED.clearData();
    // leds[cursor_position] = CRGB::White;
    // FastLED.show();

    // delay(500);

    // FastLED.clearData();
    // FastLED.show();

    // delay(500);

    // tick_length = STARTING_TICK_LEN;
    // cursor_position = 1;
    // target_position = 1;    

    game_state_start();
}

uint8_t position_led_state = 0;

int8_t get_num_overlapping() {
    // stupid brute force solution because worst possible case would be like 10x10=100
    
    int8_t num_overlapping = 0;

    for(int i = target_position; i < target_position + target_size; i++) {
        for(int j = cursor_position; j < cursor_position + cursor_size; j++) {
            // FastLED.clearData();
            // leds[i] = CRGB::Red;
            // leds[j] = CRGB::Blue;
            // FastLED.show();
            // delay(100);
            if((i % 38) == (j % 38)) num_overlapping++;
        }
    }

    return num_overlapping;
}

void game_state_loop() {

    // if(digitalRead(SPIN_BUTTON_PIN) == LOW) {
    //     if(cursor_position == target_position) {
    //         // successfully stacked
    //         target_position++;

    //         tick_length -= TICK_LEN_STEP;

    //         cursor_position = (target_position + 1) % NUM_LEDS;

    //         // game tick length should also be randomized here

    //         if(target_position == NUM_LEDS + 1) { // win state
    //             game_state_win();
    //         } else {
    //             FastLED.clearData();
    //             for(int i = 0; i < target_position; i++) {
    //                 leds[i] = CRGB::Red;
    //             }
    //             FastLED.show();

    //             while(digitalRead(SPIN_BUTTON_PIN) == LOW) {}

    //             delay(500);

    //             FastLED.clearData();
    //             for(int i = 0; i < target_position; i++) {
    //                 leds[i] = CRGB::Red;
    //             }
    //             leds[cursor_position - 1] = CRGB::Blue;
    //             FastLED.show();

    //             delay(tick_length);
    //         }
    //     } else {
    //         game_state_lost();
    //     }
    // }

    if(millis() > millis_last_target_blink_tick + 2 || digitalRead(SPIN_BUTTON_PIN) == LOW) {
        millis_last_target_blink_tick = millis();
        
        // target_led_state = !target_led_state;
        target_led_brightness += target_led_brightness_direction;

        if(target_led_brightness == 0) {
            target_led_brightness_direction = 1;
            target_led_brightness = 1;
        }
        if(target_led_brightness == target_led_max_brightness) {
            target_led_brightness_direction = -1;
            target_led_brightness = target_led_max_brightness - 1;
        }

        FastLED.clearData();
        for(int i = 0; i < target_position; i++) {
            // leds[(i) % NUM_LEDS] = CRGB::Red;
            leds[(i) % NUM_LEDS] = CRGB(64, 0, 0);
        }
        // if(target_led_state) {
        //     for(int i = target_position; i < target_position + target_size; i++) {
        //         leds[(i) % NUM_LEDS] = CRGB::Magenta;
        //     }
        // }
        for(int i = target_position; i < target_position + target_size; i++) {
            // leds[(i) % NUM_LEDS] = CRGB::Magenta;
            leds[(i) % NUM_LEDS] = CRGB(target_led_brightness, 0, 0);
        }
        for(int i = cursor_position; i < cursor_position + cursor_size; i++) {
            leds[(i) % NUM_LEDS] = CRGB::Blue;
        }
        FastLED.show();
    }

    if(digitalRead(SPIN_BUTTON_PIN) == LOW) {
        int8_t overlapping = get_num_overlapping();

        while(digitalRead(SPIN_BUTTON_PIN) == LOW) {}
        delay(500);

        if(target_position == 38 && overlapping > 0) {
            game_state_win();
            return;
        }
        if(overlapping == 0) {
            game_state_lost();
            return;
        }

        turn_number++;
        target_position += target_size;
        cursor_position = target_position + target_size;
        cursor_size = overlapping;
        target_size = target_size_steps[turn_number];
        tick_length = tick_len_steps[turn_number];

        return;
    }


    if(millis() > millis_last_tick + tick_length) {
        millis_last_tick = millis();

        cursor_position = (cursor_position + 1) % NUM_LEDS;
    }


    // for(int i = 0; i < target_position; i++) {
    //     for(int j = 0; j < target_size; j++) {
    //         leds[(target_position + j) % NUM_LEDS] = CRGB::Red;
    //     }
    // }
    // for(int i = 0; i < target_position; i++) {
    //     leds[(i) % NUM_LEDS] = CRGB::Red;
    // }
    // for(int i = target_position; i < target_position + target_size; i++) {
    //     leds[(i) % NUM_LEDS] = CRGB::Magenta;
    // }
    // if(position_led_state < 4) {
    //     leds[target_position] = CRGB::DarkBlue;
    // }
    // position_led_state = (position_led_state + 1) % 8;
    // i like the idea of it being purple when it overlaps but it makes it kinda hard to read
    // leds[cursor_position] = (cursor_position < target_position) ? CRGB::DarkMagenta : CRGB::Blue;
    // leds[cursor_position] =  CRGB::Blue;
    // for(int i = cursor_position; i < cursor_position + cursor_size; i++) {
    //     leds[(i) % NUM_LEDS] = CRGB::Green;
    // }
    
    // FastLED.show();

}

void run_game(CRGB* l) {
    leds = l;

    game_state_start();

    while(true) {
        game_state_loop();
    }
}

}
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
int8_t bound_min = 8;
int8_t bound_max = 29;
int8_t cursor_position = bound_min + 2;
int8_t cursor_size = 5;
int8_t cursor_delta = 1;
// int8_t target_position = 14;
// int8_t target_size = 5;

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
    5, 5, 5, 3, 3, 3, 3, 1, 1, 1, 1
};
const uint8_t tick_len_steps[] = {
    100, 90, 80, 70, 60, 50, 40, 40, 40, 30, 20
};

const int8_t MAX_TURN = sizeof(target_size_steps) / sizeof(target_size_steps[0]) - 1;
int8_t turn_number = 0;
int8_t target_size = 5;
int8_t score = 0;

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

    // target_position = 0;
    turn_number = 0;
    target_size = target_size_steps[turn_number];
    // cursor_position = target_position + target_size;
    tick_length = STARTING_TICK_LEN;
    // target_led_state = true;
    target_led_brightness = target_led_max_brightness;
    target_led_brightness_direction = -1;
    // target_led_max_brightness = FastLED.getBrightness();
    cursor_position = bound_min + 2;
    cursor_size = 5;
    score = 0;
}

unsigned long millis_last_tick = 0;
unsigned long millis_last_target_blink_tick = 0;

uint8_t binary_converted[] = {1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, };
uint32_t color_index = 255;
uint8_t display_size = 36;
uint8_t display_offset = 1; 
uint32_t binary_offset = 0;

void game_state_win() {
    for(int i = 0; i < 3; i++) {
        FastLED.clearData();
        FastLED.showColor(CRGB::Blue);

        delay(500);

        FastLED.clearData();
        FastLED.show();

        delay(500);
    }

    // maybe in the future have this unlock a new animation but im cooked 
    // and the deadline draws neigh so for now it just restarts the game
    //special animation while turned on though
    uint32_t color_index = 255;
    uint8_t selected_led = 0;
    unsigned long millis_last_velocity_change = millis();
    unsigned long millis_last_tick = millis();
    const uint8_t min_velocity = 50;
    const uint8_t max_velocity = 5;
    uint8_t next_velocity_update_time = 30;
    uint8_t velocity = min_velocity;

    while(true) {
        if(millis() > millis_last_velocity_change + next_velocity_update_time) {
            millis_last_velocity_change = millis();
            if(digitalRead(SPIN_BUTTON_PIN) == LOW) { //held
                next_velocity_update_time = 30;
                velocity--;
            } else {
                next_velocity_update_time = 80;
                velocity++;
            }
            if(velocity >= min_velocity) velocity = min_velocity;
            if(velocity <= max_velocity) velocity = max_velocity;
        }

        if(millis() > millis_last_tick + velocity)  {
            millis_last_tick = millis();

            FastLED.clearData();

            leds[selected_led] = util::index_to_color(color_index);
            leds[(selected_led+12) % NUM_LEDS] = util::index_to_color(color_index);
            leds[(selected_led+24) % NUM_LEDS] = util::index_to_color(color_index);

            FastLED.show();

            selected_led = (selected_led + 1) % NUM_LEDS;

            color_index += 6;
            if(color_index >= util::COLOR_MAX) color_index = 255;
        }
    }
}

void game_state_lost() {
    delay(500);
    FastLED.clearData();
    FastLED.show();
    delay(500);
}

uint8_t position_led_state = 0;

int8_t get_num_overlapping() {
    // stupid brute force solution because worst possible case would be like 10x10=100
    
    uint8_t target_start = 0;
    switch(target_size) {
        case 5:
            target_start = 17;
            break;
        case 3:
            target_start = 18;
            break;
        case 1: 
            target_start = 19;
            break;
    }

    int8_t num_overlapping = 0;

    for(int i = target_start; i < target_start + target_size; i++) {
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

bool game_state_loop() {

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
        leds[bound_min] = CRGB::White;
        leds[bound_max] = CRGB::White;
        uint8_t target_start = 0;
        switch(target_size) {
            case 5:
                target_start = 17;
                break;
            case 3:
                target_start = 18;
                break;
            case 1: 
                target_start = 19;
                break;
        }
        for(int i = target_start; i < target_start + target_size; i++) {
            leds[(i) % NUM_LEDS] = CRGB(target_led_brightness, 0, 0);
        }
        for(int i = cursor_position; i < cursor_position + cursor_size; i++) {
            leds[(i) % NUM_LEDS] = CRGB::Blue;
        }
        for(int i = 0; i < turn_number; i++) {
            leds[((NUM_LEDS - 5) + i) % NUM_LEDS] = CRGB::White;
        }
        FastLED.show();
    }

    if(digitalRead(SPIN_BUTTON_PIN) == LOW) {
        int8_t overlapping = get_num_overlapping();

        while(digitalRead(SPIN_BUTTON_PIN) == LOW) {}
        delay(500);

        if(turn_number == MAX_TURN && overlapping > 0) {
            game_state_win();
            return true;
        }
        if(overlapping == 0) {
            game_state_lost();
            return true;
        }

        turn_number++;
        target_size = target_size_steps[turn_number];
        tick_length = tick_len_steps[turn_number];
        cursor_size = overlapping;
        cursor_position = bound_min + 2;
        if(random(2) == 1) {
            cursor_position = bound_max - (cursor_size + 1);
        }
        // randomize cursor position 
        // cursor_position = random(bound_min + 2, bound_max - cursor_size);

        return false;
    }


    if(millis() > millis_last_tick + tick_length) {
        millis_last_tick = millis();

        if(cursor_position <= bound_min + 1 || cursor_position + cursor_size >= bound_max) {
            cursor_delta = -1 * cursor_delta;
        }
        // cursor_position = (cursor_position + 1) % NUM_LEDS;
        cursor_position = (cursor_position + cursor_delta);
    }


    return false;

}

void run_game(CRGB* l) {
    leds = l;

    FastLED.clear();
    FastLED.showColor(CRGB::Green);

    while(digitalRead(SPIN_BUTTON_PIN) == LOW) {}

    while(true) {
        game_state_start();
        // game_state_win();
        bool exit_loop = false;
        while(!exit_loop) {
            exit_loop = game_state_loop();
        }
    }
}

}
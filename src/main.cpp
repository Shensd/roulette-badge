

#include <Arduino.h>
#include <FastLED.h>
#include "pins.hpp"

#include "roulette.hpp"
#include "brightness_control.hpp"

#include "animations/rainbow.hpp"
#include "animations/rainbow_random.hpp"
#include "animations/rainbow_single.hpp"
#include "animations/june_sparks.hpp"
#include "animations/morse.hpp"
#include "animations/rainbow_wheel.hpp"
#include "animations/stacker.hpp"

typedef void(*ANIMATION_INIT_FUNC)(void);
ANIMATION_INIT_FUNC animation_inits[] = {
    rainbow::init,
    rainbow_random::init,
    rainbow_single::init,
    june_sparks::init,
    morse::init,
    rainbow_wheel::init,
    stacker::init,
};

CRGB leds[NUM_LEDS];

uint32_t animation_time_ms = 10000;
uint32_t next_animation_switch = 0;
bool do_rotate = true;

bool button_pressed = false;

void button_press() {
    button_pressed = true;
}

void setup() {
    randomSeed(analogRead(ENTROPY_1_PIN) ^ analogRead(ENTROPY_2_PIN) ^ analogRead(ENTROPY_3_PIN) ^ analogRead(ENTROPY_4_PIN));

    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(5);

    for(int i = 0; i < sizeof(animation_inits) / sizeof(ANIMATION_INIT_FUNC); i++) {
        animation_inits[i]();
    }

    pinMode(SPIN_BUTTON_PIN, INPUT_PULLUP);
    // attachInterrupt(digitalPinToInterrupt(SPIN_BUTTON_PIN), button_press, LOW);

    next_animation_switch = millis() + animation_time_ms;

    Serial.begin(115200);
}

typedef void(*ANIMATION_FUNC)(CRGB*);

uint16_t animation_index = 0; 
ANIMATION_FUNC animations[] = {
    rainbow::animation,
    rainbow_random::animation,
    rainbow_single::animation,
    rainbow_wheel::animation,
    june_sparks::animation,
    morse::animation,
    stacker::animation,
};

bool button_held = false;
// if the device is left on for 277 hours without the button being pressed
// then the device will bug out a bit the first time the button is pressed
#define DISTANT_TIME 999999999
unsigned long millis_started_holding = DISTANT_TIME;
unsigned long millis_let_go = DISTANT_TIME;

// single tap = run roulette
// double tap = change animation
// press and hold = brightness config
// im thinking first press saves current time
// if current time + 200ms passes, run roulette
// if button pressed again before current time + 200ms, change animation
// if button held for 500ms, run brightness control

// maybe double tap to switch to animation to keep? and each double tap increments to the next animation

void increment_animation() {
    animation_index = (animation_index + 1) % (sizeof(animations) / sizeof(ANIMATION_FUNC));
    next_animation_switch = millis() + animation_time_ms;
}


void loop() {
    if(do_rotate) {
        if(millis() > next_animation_switch) {
            increment_animation();
        }
    }

    bool button_is_pressed = digitalRead(SPIN_BUTTON_PIN) == LOW;
    bool button_not_pressed = !button_is_pressed;
    

    // rising edge
    if(button_is_pressed && !button_held) {
        button_held = true;
        millis_started_holding = millis();
    }

    // falling edge
    if(button_not_pressed && button_held) {
        button_held = false;

        // if it's been less than 200ms since the last press, do animation config
        if(millis() < (millis_let_go + 200) && millis() > millis_let_go) {
            do_rotate = false;
            increment_animation();

            millis_started_holding = DISTANT_TIME;
            millis_let_go = DISTANT_TIME;
        } else {
            millis_let_go = millis();
        }
    }

    // button held for more than 500ms, run brightnes configure
    if(button_held && millis() > (millis_started_holding + 500)) {
        brightness_control::interactive_configure();
        button_held = false;
        millis_started_holding = DISTANT_TIME;
        millis_let_go = DISTANT_TIME;
        if(do_rotate) {
            // if rotating, force start next animation to prevent current animation from flicking on for 1 tick
            next_animation_switch = 0;
        }
        return;
    }

    // if it has been 200ms since button pressed, run roulette
    if(button_not_pressed && millis() > (millis_let_go + 200)) {
        run_roulette();
        millis_started_holding = DISTANT_TIME;
        millis_let_go = DISTANT_TIME;
        if(do_rotate) {
            // if rotating, force start next animation to prevent current animation from flicking on for 1 tick
            next_animation_switch = 0;
            return;
        }
    }

    // if(button_pressed && millis() < (millis_let_go + 200)) {
    //     FastLED.clearData();
    //     FastLED.showColor(CRGB::Red);
    //     delay(1000);
    // }

    /*
    if(digitalRead(SPIN_BUTTON_PIN) == HIGH && button_held) {
        button_held = false;
        run_roulette();
        // force next animation and reset timer so it doens't flash current animation for just a moment
        increment_animation();
    }

    if(digitalRead(SPIN_BUTTON_PIN) == LOW && !button_held) {
        button_held = true;
        started_holding = millis();
    }

    // if button has been held for 500ms switch to brightness config mode
    if(button_held && millis() > (started_holding + 500)) {
        brightness_control::interactive_configure();
        button_held = false;
        return;
    }
    */

    animations[animation_index](leds);
}
#include "june_sparks.hpp"

namespace june_sparks {

struct glider {
    int8_t direction = 1; 
    int32_t speed = 3;
    int8_t position = 0;
    CRGB color = 0;
    uint32_t next_update_tick_ms = 0;
};
glider gliders[2];

// these are delays to move in ms so max vel being a smaller number than
// min vel (while bad naming) is not incorrect
uint8_t MAX_VELOCITY = 20;
uint8_t MIN_VELOCITY = 40;

void randomize_velocities() {
    gliders[0].speed = random(MIN_VELOCITY - (MAX_VELOCITY - 1)) + MAX_VELOCITY;
    gliders[1].speed = random(MIN_VELOCITY - (MAX_VELOCITY - 1)) + MAX_VELOCITY;
}

CRGB accepted_colors[] = {
    CRGB::Red,
    CRGB::Green,
    CRGB::Blue,
    CRGB::Purple,
    CRGB::Cyan,
    CRGB::Aqua,
    CRGB::Pink,
    CRGB::Magenta,
    CRGB::Lime
};

void randomize_colors() {
    // make sure both points have different, new colors
    CRGB glider_0_prev_color = gliders[0].color;
    CRGB glider_1_prev_color = gliders[1].color;
    do {
        gliders[0].color = accepted_colors[random(sizeof(accepted_colors) / sizeof(CRGB))];
    } while(gliders[0].color == glider_0_prev_color || gliders[0].color == glider_1_prev_color);
    do {
        gliders[1].color = accepted_colors[random(sizeof(accepted_colors) / sizeof(CRGB))];
    } while(gliders[1].color == gliders[0].color || gliders[1].color == glider_1_prev_color || gliders[1].color == glider_0_prev_color);
}

void init() {
    // for(int i = 0; i < sizeof(gliders) / sizeof(glider); i++) {
    //     gliders[i].position = random(NUM_LEDS);
    // }
    gliders[0].position = 14;
    gliders[0].direction = 1;
    gliders[1].position = 0;
    gliders[1].direction = -1;

    randomize_velocities();
    // randomize_colors();
    gliders[0].color = CRGB::Red;
    gliders[1].color = CRGB::Blue;

    gliders[0].next_update_tick_ms = millis() + gliders[0].speed;
    gliders[1].next_update_tick_ms = millis() + gliders[1].speed;
}

bool update_positions() {
    bool either_moved = false;

    for(int i = 0; i < 2; i++) {
        if(millis() < gliders[i].next_update_tick_ms) continue;

        either_moved = true;
        gliders[i].next_update_tick_ms = millis() + gliders[i].speed;

        gliders[i].position += (gliders[i].direction);

        if(gliders[i].position >= NUM_LEDS) gliders[i].position = gliders[i].position % NUM_LEDS;

        if(gliders[i].position < 0) gliders[i].position = NUM_LEDS - 1;

        if(either_moved) break; // prevent both moving on the same tick and potentialy eternally missing eachother
    }

    return either_moved;
}

bool are_colliding() {
    glider a, b; // a has positive velocity, b has negative velocity
    int a_current_pos, a_next_pos, b_current_pos, b_next_pos;

    if(gliders[0].direction > 0) {
        a = gliders[0];
        b = gliders[1];
    } else {
        a = gliders[1];
        b = gliders[0];
    }
    a_current_pos = a.position;
    b_current_pos = b.position;

    a_next_pos = a.position + (a.direction * a.speed);
    b_next_pos = b.position + (b.direction * b.speed);

    // a is moving towards 38, b is moving towards 0


    // they have collided if b's next position is an index greater than a's next position
    // AND b's current position is an index less than a's current position 

    return (b_next_pos >= a_next_pos && b_current_pos < a_current_pos);
}

void animation(CRGB* leds) {

    bool either_moved = update_positions();

    FastLED.clearData();

    // if(either_moved && are_colliding()) {
    if(either_moved && gliders[0].position == gliders[1].position) {
        // int collision_position = gliders[0].position + (gliders[0].direction);
        int collision_position = gliders[0].position;

        leds[collision_position] = CRGB::White;

        gliders[0].direction = -1 * gliders[0].direction;
        gliders[1].direction = -1 * gliders[1].direction;

        randomize_velocities();
        // randomize_colors();

        FastLED.show();

        // delay(50);
    } else {
        leds[gliders[0].position] = gliders[0].color;
        leds[gliders[1].position] = gliders[1].color;

        FastLED.show();

        // delay(10);
    }
}

}
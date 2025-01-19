#include "rainbow.hpp"

namespace rainbow {

// int16_t r = 0, g = 0, b = 0; 
// void transition_to(CRGB* leds, uint16_t red, uint16_t green, uint16_t blue, uint16_t delay_ms) {
//   uint8_t step = 6;

//   while(abs(red - r) > (step + 1) || abs(green - g) > (step + 1) || abs(blue - b) > (step + 1)) {
//     if(r > red) r -= step;
//     if(r < red) r += step;
//     if(r > 255) r = 255;
//     if(r < 0) r = 0;

//     if(g > green) g -= step;
//     if(g < green) g += step;
//     if(g > 255) g = 255;
//     if(g < 0) g = 0;

//     if(b > blue) b -= step;
//     if(b < blue) b += step;
//     if(b > 255) b = 255;
//     if(b < 0) b = 0;
    
//     for(int i = 0; i < NUM_LEDS; i++) {
//       leds[i] = CRGB((uint8_t)r, (uint8_t)g, (uint8_t)b);
//     }

//     FastLED.show();
//     delay(delay_ms);
//   }
  
// }

// void init() {}

// void animation(CRGB* leds) {
//     uint16_t delay_ms = 10;
//     transition_to(leds, 255, 0, 0, delay_ms);
//     transition_to(leds, 255, 255, 0, delay_ms);
//     transition_to(leds, 0, 255, 0, delay_ms);
//     transition_to(leds, 0, 255, 255, delay_ms);
//     transition_to(leds, 0, 0, 255, delay_ms);
//     transition_to(leds, 255, 0, 255, delay_ms);
// }

uint32_t COLOR_MAX = 255 * 7;
uint32_t color_index = 0;

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

    // FastLED.showColor(index_to_color(color_index));
    CRGB color = index_to_color(color_index);
    // for(int i = 0; i < NUM_LEDS; i++) {
    //   leds[i] = color;
    // }
    FastLED.showColor(color);

    // FastLED.show();

    color_index += 4;
    if(color_index >= COLOR_MAX) color_index = 255;

    delay(10);
}

}
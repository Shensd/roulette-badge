#include "brightness_control.hpp"

namespace brightness_control {

void interactive_configure() {
    int16_t brightness = FastLED.getBrightness();
    uint8_t brightness_tick = 5;
    uint8_t MAX_BRIGHTNESS = 25;
    uint8_t MIN_BRIGHTNESS = 5;

    bool being_held = digitalRead(SPIN_BUTTON_PIN) == LOW;
    uint32_t last_press = millis();

    FastLED.clearData();
    FastLED.showColor(CRGB::White);

    while(true) {
        uint8_t btn_status = digitalRead(SPIN_BUTTON_PIN);

        if(btn_status == LOW && !being_held) {
            being_held = true;

            brightness = (brightness + brightness_tick) % MAX_BRIGHTNESS;
            if(brightness == 0) brightness = MIN_BRIGHTNESS;
            FastLED.setBrightness(brightness);

            FastLED.clearData();
            FastLED.showColor(CRGB::White);

            last_press = millis();
        }

        if(btn_status == HIGH && being_held) {
            being_held = false;
            last_press = millis();
        }

        if(millis() > last_press + 3000 && !being_held) {
            break;
        }
        
    }
}

}
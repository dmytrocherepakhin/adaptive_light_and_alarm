#pragma once
#include <Arduino.h>

class Button
{
public:
    void init(u8_t pin);
    void poll(u32_t now);
    bool get_state();

private:
    u32_t debounceDelay = 50;
    u8_t pin;
    bool buttonState = false;
    bool lastReading = LOW;
    bool stableState = false;
    u32_t lastChangeTime = 0;
};

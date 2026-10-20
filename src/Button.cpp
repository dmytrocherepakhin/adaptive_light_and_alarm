#include "Button.h"
#include <Arduino.h>

void Button::init(uint8_t buttonPin)
{
    pin = buttonPin;
    pinMode(pin, INPUT);
}

void Button::poll(u32_t now)
{
    bool reading = digitalRead(pin);

    if (reading != lastReading)
    {
        lastReading = reading;
        lastChangeTime = now;
    }

    if (now - lastChangeTime >= debounceDelay && reading != stableState)
    {
        stableState = reading;
        if (stableState == HIGH)
        {
            buttonState = !buttonState;
        }
    }
}

bool Button::get_state()
{
    return buttonState;
}
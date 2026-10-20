#pragma once
#include <Arduino.h>

class Led
{
public:
    void init(u8_t pin);
    void set_state(bool state);
    void set_duty(u8_t duty);

private:
    u8_t pin;
};
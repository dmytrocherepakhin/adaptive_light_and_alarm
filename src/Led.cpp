#include "Led.h"
#include<Arduino.h>

void Led::init(u8_t ledPin)
{
    pin = ledPin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Led::set_state(bool state)
{
    digitalWrite(pin, state ? HIGH : LOW);
}

void Led::set_duty(u8_t duty)
{
    analogWrite(pin, duty);
}
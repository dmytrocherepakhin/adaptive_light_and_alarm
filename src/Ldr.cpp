#include "Ldr.h"
#include <Arduino.h>

void Ldr::init(uint8_t ldrPin)
{
    pin = ldrPin;
    pinMode(pin, INPUT);
    initialValue = analogRead(pin);
    filterInit(initialValue);
    adcValue = initialValue;
    lastReadTime = millis();
}

void Ldr::filterInit(u16_t initialValue)
{
    sum = 0;
    idx = 0;

    for (u8_t i = 0; i < LDR_FILTER_SIZE; i++)
    {
        readings[i] = initialValue;
        sum += initialValue;
    }
}

u16_t Ldr::movingAvg(u16_t rawData)
{
    sum -= readings[idx];
    readings[idx] = rawData;
    sum += rawData;
    idx++;
    if (idx >= LDR_FILTER_SIZE)
    {
        idx = 0;
    }

    return sum / LDR_FILTER_SIZE;
}

void Ldr::poll(uint32_t now)
{
    if (now - lastReadTime >= readPeriod)
    {
        u16_t rawValue = analogRead(pin);
        adcValue = movingAvg(rawValue);
    }
}

u32_t Ldr::get_value()
{
    return adcValue;
}
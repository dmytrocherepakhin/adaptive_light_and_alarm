#pragma once
#include <Arduino.h>

const u8_t LDR_FILTER_SIZE = 8;

class Ldr
{
public:
    void init(u8_t pin);
    void poll(u32_t now);
    u32_t get_value();

private:
    u8_t pin;

    u16_t readings[LDR_FILTER_SIZE] = {};
    u8_t idx = 0;
    u32_t sum = 0;

    u32_t initialValue = 0;
    u32_t adcValue = 0;
    u32_t lastReadTime = 0;
    u32_t readPeriod = 100;

    void filterInit(u16_t initialValue);
    u16_t movingAvg(u16_t rawData);
};
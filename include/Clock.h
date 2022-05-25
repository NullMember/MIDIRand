#pragma once

#include <Arduino.h>

#ifndef CLOCK_H
#define CLOCK_H

class Clock
{
private:
    Clock() {};
    void (*clock_callback)() = nullptr;
    uint8_t tempo = 120;
    uint8_t tempo_divider = 4;
    int32_t interval_us = 0;
    int32_t last_us = 0;
    int32_t target_us = 0;
    int32_t error_us = 0;
    uint8_t random_p = 1;
    int32_t random_us = 0;
public:
    Clock(void (*callback)(), uint8_t bpm = 120);
    ~Clock() {};
    void update();
    void set_tempo(uint8_t);
    uint8_t get_tempo();
    void set_divider(uint8_t);
    uint8_t get_divider();
    void set_random(uint8_t percentage);
    uint8_t get_random();
    // void set_callback(void (*callback)());
};

#endif //CLOCK
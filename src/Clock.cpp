#include <Clock.h>

Clock::Clock(void (*callback)(), uint8_t bpm) {
    clock_callback = callback;
    set_tempo(bpm);
}

void Clock::update() {
    int32_t current_us = micros();
    if (target_us + last_us <= current_us) {
        clock_callback();
        error_us = (current_us - last_us) - interval_us;
        target_us = (interval_us - error_us) + (((int32_t)(micros() % random_p) - (int32_t)(random_p >> 1)) * random_us);
        last_us = current_us;
    }
}

void Clock::set_divider(uint8_t divider) {
    tempo_divider = divider;
    set_tempo(tempo);
}

uint8_t Clock::get_divider() {
    return tempo_divider;
}

void Clock::set_tempo(uint8_t bpm) {
    tempo = bpm;
    interval_us = (60000000ul / (uint32_t)tempo) / tempo_divider;
    random_us = interval_us >> 8;
    target_us = interval_us;
    // last_us = micros();
}

uint8_t Clock::get_tempo() {
    return tempo;
}

void Clock::set_random(uint8_t percentage) {
    random_p = percentage;
    random_us = interval_us >> 8;
    target_us = interval_us;
    // last_us = micros();
}

uint8_t Clock::get_random() {
    return random_p;
}
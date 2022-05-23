#pragma once

#include <Arduino.h>
#include <usbmidi.h>
#include <consts.h>

#ifndef MIDI_SENDER_H
#define MIDI_SENDER_H

inline void send_note_off(uint8_t note, uint8_t velocity, uint8_t channel = 0) {
    USBMIDI.write(MIDI_NOTE_OFF | (channel & 0xF));
    USBMIDI.write(note);
    USBMIDI.write(velocity);
}

inline void send_note_on(uint8_t note, uint8_t velocity, uint8_t channel = 0) {
    USBMIDI.write(MIDI_NOTE_ON | (channel & 0xF));
    USBMIDI.write(note);
    USBMIDI.write(velocity);
}

inline void send_aftertouch(uint8_t key, uint8_t pressure, uint8_t channel = 0) {
    USBMIDI.write(MIDI_AFTERTOUCH | (channel & 0xF));
    USBMIDI.write(key);
    USBMIDI.write(pressure);
}

inline void send_control_change(uint8_t control, uint8_t value, uint8_t channel = 0) {
    USBMIDI.write(MIDI_CONTROL_CHANGE | (channel & 0xF));
    USBMIDI.write(control);
    USBMIDI.write(value);
}

inline void send_program_change(uint8_t program, uint8_t channel = 0) {
    USBMIDI.write(MIDI_PROGRAM_CHANGE | (channel & 0xF));
    USBMIDI.write(program);
}

inline void send_channel_pressure(uint8_t pressure, uint8_t channel = 0) {
    USBMIDI.write(MIDI_CHANNEL_PRESSURE | (channel & 0xF));
    USBMIDI.write(pressure);
}

inline void send_pitch_bend(int16_t bend, uint8_t channel = 0) {
    bend += 0x2000;
    uint8_t lsb = bend & 0x7F;
    uint8_t msb = (bend >> 7) & 0x7F;
    USBMIDI.write(MIDI_PITCH_BEND | (channel & 0xF));
    USBMIDI.write(lsb);
    USBMIDI.write(msb);
}

#endif //MIDI_SENDER_H
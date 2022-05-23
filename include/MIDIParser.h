#pragma once

#include <Arduino.h>
#include <consts.h>

#ifndef MIDI_PARSER_H
#define MIDI_PARSER_H

class MIDIParser
{
private:
    uint8_t* buffer = nullptr;
    uint8_t buffer_length = 0;
    uint8_t buffer_remaining = 255;
    // void (*note_off_callback)(uint8_t, uint8_t, uint8_t) = nullptr;
    void (*note_on_callback)(uint8_t, uint8_t, uint8_t) = nullptr;
    // void (*aftertouch_callback)(uint8_t, uint8_t, uint8_t) = nullptr;
    // void (*control_change_callback)(uint8_t, uint8_t, uint8_t) = nullptr;
    // void (*program_change_callback)(uint8_t, uint8_t) = nullptr;
    // void (*channel_pressure_callback)(uint8_t, uint8_t) = nullptr;
    // void (*pitch_bend_callback)(int16_t, uint8_t) = nullptr;
public:
    MIDIParser(uint8_t buffer_size = 32);
    ~MIDIParser();
    void process(uint8_t data);
    // void set_note_off_callback(void (*callback)(uint8_t, uint8_t, uint8_t));
    void set_note_on_callback(void (*callback)(uint8_t, uint8_t, uint8_t));
    // void set_aftertouch_callback(void (*callback)(uint8_t, uint8_t, uint8_t));
    // void set_control_change_callback(void (*callback)(uint8_t, uint8_t, uint8_t));
    // void set_program_change_callback(void (*callback)(uint8_t, uint8_t));
    // void set_channel_pressure_callback(void (*callback)(uint8_t, uint8_t));
    // void set_pitch_bend_callback(void (*callback)(int16_t, uint8_t));
};


#endif //MIDI_PARSER_H
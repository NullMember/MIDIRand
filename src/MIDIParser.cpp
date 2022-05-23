#include <MIDIParser.h>

MIDIParser::MIDIParser(uint8_t buffer_size)
{
    buffer = new uint8_t[buffer_size];
}

MIDIParser::~MIDIParser()
{
    delete[] buffer;
}

void MIDIParser::process(const uint8_t data) {
    if (data & 0x80) {
        const uint8_t cmd = data & 0xF0;
        buffer_length = 0;
        switch (cmd) {
            // case MIDI_NOTE_OFF:
            case MIDI_NOTE_ON:
            // case MIDI_AFTERTOUCH:
            // case MIDI_CONTROL_CHANGE:
            // case MIDI_PITCH_BEND:
                buffer[buffer_length++] = data;
                buffer_remaining = 2;
                break;
            // case MIDI_PROGRAM_CHANGE:
            // case MIDI_CHANNEL_PRESSURE:
            //     buffer[buffer_length++] = data;
            //     buffer_remaining = 1;
            //     break;
            default:
                buffer_remaining = 255;
                break;
        }
    }
    else {
        if (buffer_remaining--) {
            buffer[buffer_length++] = data;
        }
    }
    if (buffer_remaining == 0) {
        const uint8_t cmd = buffer[0] & 0xF0;
        const uint8_t chn = buffer[0] & 0x0F;
        switch (cmd) {
            // case MIDI_NOTE_OFF:
            //     if (note_off_callback != nullptr)
            //     {
            //         note_off_callback(buffer[1], buffer[2], chn);
            //     }
            //     break;
            case MIDI_NOTE_ON:
                if (note_on_callback != nullptr)
                {
                    note_on_callback(buffer[1], buffer[2], chn);
                }
                break;
            // case MIDI_AFTERTOUCH:
            //     if (aftertouch_callback != nullptr)
            //     {
            //         aftertouch_callback(buffer[1], buffer[2], chn);
            //     }
            //     break;
            // case MIDI_CONTROL_CHANGE:
            //     if (control_change_callback != nullptr)
            //     {
            //         control_change_callback(buffer[1], buffer[2], chn);
            //     }
            //     break;
            // case MIDI_PROGRAM_CHANGE:
            //     if (program_change_callback != nullptr)
            //     {
            //         program_change_callback(buffer[1], chn);
            //     }
            //     break;
            // case MIDI_CHANNEL_PRESSURE:
            //     if (channel_pressure_callback != nullptr)
            //     {
            //         channel_pressure_callback(buffer[1], chn);
            //     }
            //     break;
            // case MIDI_PITCH_BEND:
            //     if (pitch_bend_callback != nullptr)
            //     {
            //         int16_t bend = ((buffer[1] & 0x7F) | ((buffer[2] & 0x7F) << 7)) - 0x2000; 
            //         pitch_bend_callback(bend, chn);
            //     }
            //     break;
            default:
                break;
        }
    }
}

// void MIDIParser::set_note_off_callback(void (*callback)(uint8_t, uint8_t, uint8_t)) {
//     note_off_callback = callback;
// }

void MIDIParser::set_note_on_callback(void (*callback)(uint8_t, uint8_t, uint8_t)) {
    note_on_callback = callback;
}

// void MIDIParser::set_aftertouch_callback(void (*callback)(uint8_t, uint8_t, uint8_t)) {
//     aftertouch_callback = callback;
// }

// void MIDIParser::set_control_change_callback(void (*callback)(uint8_t, uint8_t, uint8_t)) {
//     control_change_callback = callback;
// }

// void MIDIParser::set_program_change_callback(void (*callback)(uint8_t, uint8_t)) {
//     program_change_callback = callback;
// }

// void MIDIParser::set_channel_pressure_callback(void (*callback)(uint8_t, uint8_t)) {
//     channel_pressure_callback = callback;
// }

// void MIDIParser::set_pitch_bend_callback(void (*callback)(int16_t, uint8_t)) {
//     pitch_bend_callback = callback;
// }
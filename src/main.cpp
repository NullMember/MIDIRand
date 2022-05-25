#include <Arduino.h>
#include <EEPROM.h>
#include <usbmidi.h>
#include <MIDIParser.h>
#include <MIDISender.h>
#include <Clock.h>

#define MIDI_BUFFER_LENGTH  8
#define CHANNELS            4
#define S_NOTES             16
#define R_NOTES             8
#define S_EEPROM_START      0
#define R_EEPROM_START      128
#define O_EEPROM_START      256
#define E_TEMPO_OFFSET      0
#define E_DIVIDER_OFFSET    1
#define E_RANDOM_OFFSET     2
#define S_OFFSET            0
#define R_OFFSET            4

uint8_t s_notes[CHANNELS][S_NOTES + 1];
uint8_t s_last_note[CHANNELS] = {0, 0, 0, 0};
uint8_t s_pointers[CHANNELS] = {0, 0, 0, 0};

uint8_t r_notes[CHANNELS][R_NOTES + 1];
uint8_t r_pointers[CHANNELS] = {0, 0, 0, 0};

void note_on_callback(uint8_t, uint8_t, uint8_t);
void clock_callback();
inline void load_eeprom();
inline void save_eeprom();

MIDIParser parser(MIDI_BUFFER_LENGTH);
Clock clock(clock_callback);

void setup()
{
    parser.set_note_on_callback(note_on_callback);
    load_eeprom();
}

void loop()
{
    // put your main code here, to run repeatedly:
    clock.update();
    USBMIDI.poll();
    while (USBMIDI.available())
    {
        parser.process(USBMIDI.read());
    }
}

void clock_callback() {
    for (uint8_t i = 0; i < CHANNELS; i++)
    {
        if (s_notes[i][0]) {
            s_pointers[i] = s_notes[i][s_pointers[i] + 2] ? ((s_pointers[i] + 1) % S_NOTES) : 0;
            uint8_t note = s_notes[i][s_pointers[i] + 1];
            if (note < 127)
            {
                send_note_on(s_last_note[i], 0, i);
                if (note > 0)
                {
                    s_last_note[i] = note;
                    send_note_on(s_last_note[i], (micros() % 64) + 64, i);
                }
            }
        }

        if (r_notes[i][0]) {
            send_note_on(r_notes[i][r_pointers[i] + 1], 0, i + R_OFFSET);
            r_pointers[i] = (micros() >> 2) % 8;
            send_note_on(r_notes[i][r_pointers[i] + 1], (micros() % 64) + 64, i + R_OFFSET);
        }
    }
}

void note_on_callback(uint8_t note, uint8_t velocity, uint8_t channel) {
    if (channel < 4) {
        if (note < S_NOTES + 1) {
            s_notes[channel][note] = velocity;
        }
        else if (note == S_NOTES + 1) {
            for (uint8_t i = 1; i < S_NOTES + 1; i++) {
                s_notes[channel][i] = (uint8_t)((int8_t)s_notes[channel][i] + ((int8_t)velocity - 64)) % 128;
            }
        }
    }
    else if (channel < 8) {
        if (note < R_NOTES + 1) {
            r_notes[channel - R_OFFSET][note] = velocity;
        }
        else if (note == R_NOTES + 1) {
            for (uint8_t i = 1; i < R_NOTES + 1; i++) {
                r_notes[channel - R_OFFSET][i] = (uint8_t)((int8_t)r_notes[channel - R_OFFSET][i] + ((int8_t)velocity - 64)) % 128;
            }
        }
    }
    else if (channel == 8) {
        clock.set_tempo(velocity + 30);
    }
    else if (channel == 9) {
        clock.set_random(velocity + 1);
    }
    else if (channel == 10) {
        clock.set_divider(velocity + 1);
    }
    else if (channel == 15) {
        if (note == 127) {
            if (velocity == 127) {
                save_eeprom();
            }
        }
    }
}

inline void load_eeprom() {
    for (uint8_t i = 0; i < CHANNELS; i++){
        for (uint8_t j = 0; j < S_NOTES + 1; j++)
        {
            s_notes[i][j] = EEPROM.read(S_EEPROM_START + (i * (S_NOTES + 1)) + j);
        }
        for (uint8_t j = 0; j < R_NOTES + 1; j++)
        {
            r_notes[i][j] = EEPROM.read(R_EEPROM_START + (i * (R_NOTES + 1)) + j);
        }
    }
    clock.set_tempo(EEPROM.read(O_EEPROM_START + E_TEMPO_OFFSET));
    clock.set_divider(EEPROM.read(O_EEPROM_START + E_DIVIDER_OFFSET));
    clock.set_random(EEPROM.read(O_EEPROM_START + E_RANDOM_OFFSET));
}

inline void save_eeprom() {
    for (uint8_t i = 0; i < CHANNELS; i++){
        for (uint8_t j = 0; j < S_NOTES + 1; j++)
        {
            EEPROM.update(S_EEPROM_START + (i * (S_NOTES + 1)) + j, s_notes[i][j]);
        }
        for (uint8_t j = 0; j < R_NOTES + 1; j++)
        {
            EEPROM.update(R_EEPROM_START + (i * (R_NOTES + 1)) + j, r_notes[i][j]);
        }
    }
    EEPROM.update(O_EEPROM_START + E_TEMPO_OFFSET, clock.get_tempo());
    EEPROM.update(O_EEPROM_START + E_DIVIDER_OFFSET, clock.get_divider());
    EEPROM.update(O_EEPROM_START + E_RANDOM_OFFSET, clock.get_random());
}
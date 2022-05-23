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

uint8_t s_notes[CHANNELS][S_NOTES + 1];
uint8_t s_last_note[CHANNELS] = {0, 0, 0, 0};
uint8_t s_pointers[CHANNELS] = {0, 0, 0, 0};

uint8_t r_notes[CHANNELS][R_NOTES + 1];
uint8_t r_pointers[CHANNELS] = {0, 0, 0, 0};

void note_on_callback(uint8_t, uint8_t, uint8_t);
void clock_callback();

MIDIParser parser(MIDI_BUFFER_LENGTH);
Clock clock(clock_callback);

void setup()
{
    parser.set_note_on_callback(note_on_callback);
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
                    send_note_on(s_last_note[i], (rand() % 64) + 64, i);
                }
            }
        }

        if (r_notes[i][0]) {
            send_note_on(r_notes[i][r_pointers[i] + 1], 0, i + 4);
            r_pointers[i] = (micros() >> 2) % 8;
            send_note_on(r_notes[i][r_pointers[i] + 1], (micros() % 64) + 64, i + 4);
        }
    }
}

void note_on_callback(uint8_t note, uint8_t velocity, uint8_t channel) {
    if (channel < 4) {
        if (note < S_NOTES + 1) {
            s_notes[channel][note] = velocity;
            EEPROM.update(S_EEPROM_START + (channel * (S_NOTES + 1)) + note, velocity);
        }
    }
    else if (channel < 8) {
        if (note < R_NOTES + 1) {
            r_notes[channel - 4][note] = velocity;
            EEPROM.update(R_EEPROM_START + ((channel - 4) * (R_NOTES + 1)) + note, velocity);
        }
    }
    else if (channel == 8) {
        clock.set_tempo(velocity + 30);
    }
    else if (channel == 9) {
        clock.set_random(velocity);
    }
}
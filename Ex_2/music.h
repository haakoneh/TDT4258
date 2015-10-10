#ifndef MUSIC_H
#define MUSIC_H

#include <stdint.h>

#include "note_data.h"
#include "song_data.h"

extern uint32_t song_iterator;
extern uint16_t note_counter;
extern uint16_t counter;
extern Song* current_song;
extern uint16_t current_note_length;


void music_set_song(Song*, uint16_t);

void music_play_song(Song*, uint16_t);

void music_note_to_dac(Note* n_temp, int offset);

void music_cleanup();

#endif


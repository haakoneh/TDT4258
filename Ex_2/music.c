#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "dac.h"
#include "timer.h"
#include "gpio.h"
#include "music.h"
#include "main.h"

uint32_t song_iterator = 0;
uint16_t note_counter = 0;
uint16_t counter = 0;

Song* current_song;
uint16_t current_note_length;

/***************************************************************
* FUNCTION NAME    main
*
* DESCRIPTION:
*
*		Sets up sleep, gpio, NVIC, play a startup song, 
*	   	and wait for interrupt
*
****************************************************************/

void music_set_song(Song* song, uint16_t note_length) {
    current_song = song;
    current_note_length = note_length;

}

/***************************************************************
* FUNCTION NAME    music_play_song
*
* DESCRIPTION:
*
*		Set up current_song, sleep, dac, timer. Then start timer
*
****************************************************************/

void music_play_song(Song* song, uint16_t note_length) {
    music_set_song(song, note_length);
    sleep_setup(0b010);
    dac_setup();
    timer_setup();
    timer_start();
}

/***************************************************************
* FUNCTION NAME    music_cleanup
*
* DESCRIPTION:
*
*		Stop timer and resets all counters connected to the 
*		traversing of song and note arrays 
*
****************************************************************/

void music_cleanup() {
    timer_stop();
    song_iterator = 0;
    counter = 0;
    note_counter = 0;
}

/***************************************************************
* FUNCTION NAME    music_note_to_dac
*
* DESCRIPTION:
*
*		Send note frequency to dac function, which sends given 
*		frequency to output  
*
****************************************************************/

void music_note_to_dac(Note* n_temp, int offset){
    dac_write(n_temp->samples[offset]);
}


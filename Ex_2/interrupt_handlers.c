#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "dac.h"
#include "timer.h"
#include "gpio.h"
#include "music.h"
#include "main.h"

/***************************************************************
* FUNCTION NAME    __attribute__ ((interrupt)) TIMER1_IRQHandler
*
* DESCRIPTION:
*
*		Traverses through the note arrays in the current song,
*		every 317 clock cycle, checks if at end of current note, or 
*		song 
*
****************************************************************/

void __attribute__ ((interrupt)) TIMER1_IRQHandler() {
    // Clear interrupt flag 
    *TIMER1_IFC = 1;
    
	//check if at end of note, may go to next note
	if ( counter >= current_note_length ) {
        counter = 0;
        note_counter++;
    } else {
        counter++;
    }
	
	//check if at end of song, resets values if true and end function
    if ( note_counter >= current_song->length ) {
        music_cleanup();
        gpio_led_clear();
        dac_disable();
        timer_disable();
        sleep_setup(0b110);
        return;
    }
    
	//creates temp pointer to note, play that frequency
    Note* n_temp = current_song->notes[note_counter];
    int offset = (song_iterator % n_temp->length);
    music_note_to_dac(n_temp, offset);
    song_iterator++;
}

/***************************************************************
* FUNCTION NAME    GPIO_Handler
*
* DESCRIPTION:
*
*		Resets music counters, clears gpio and turn on led,
* 		then plays the chosen song
*
****************************************************************/

void GPIO_Handler() {
    //resets appropriate counters and io
	music_cleanup();
    gpio_clear();
    gpio_button_to_led();  
    
	//checks which song have been chosen
    int button_case = gpio_map_input();
    switch(button_case){
        case 1:
            music_play_song(&JUMP, 0x027F);
            break;
        case 2:
            music_play_song(&PEWPEW, 0x3FF);
            break;
        case 3:
             music_play_song(&ONEUP, 0x71f);
            break;
        default:
             music_play_song(&ONEUP, 0x71f);
            break;
    }
}

/***************************************************************
* FUNCTION NAME    __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler
*
* DESCRIPTION:
*
*		Takes care of input from even sources 
*
****************************************************************/

void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() {
    GPIO_Handler();
}

/***************************************************************
* FUNCTION NAME    __attribute__ ((interrupt)) GPIO_ODD_IRQHandler
*
* DESCRIPTION:
*
*		Takes care of input from odd sources 
*
****************************************************************/

void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() {
    GPIO_Handler();
}

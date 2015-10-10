#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "dac.h"
#include "timer.h"
#include "gpio.h"
#include "music.h"
#include "ex2.h"

/***************************************************************
* FUNCTION NAME    main
*
* DESCRIPTION:
*
*		Sets up sleep, gpio, NVIC, play a startup song, 
*	   	and wait for interrupt
*
****************************************************************/

int main() {
    sleep_setup(0b110);
    gpio_setup();
    NVIC_setup();
     music_play_song(&ONEUP, 0x71f);
    while(1) {
		__asm__("wfi");
	}
    return 0;
}

/***************************************************************
* FUNCTION NAME    NVIC_setup
*
* DESCRIPTION:
*
*		Sets up NVIC
*
****************************************************************/

void NVIC_setup() {
    *ISER0 = 0x1802;
}

/***************************************************************
* FUNCTION NAME    sleep_setup
*
* DESCRIPTION:
*
*		Puts the cpu to sleep by writing 1 to the second bit 
*		in SCR, use arg for more flexibility 
*
****************************************************************/

void sleep_setup(int arg) {
    *SCR = arg;
}

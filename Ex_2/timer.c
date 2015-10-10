#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"


/* The period between sound samples, in clock cycles */
#define SAMPLE_PERIOD 317

/***************************************************************
* FUNCTION NAME    timer_setup
*
* DESCRIPTION:
*
*		Sets up timer by enable clock, defines period and 
*		enable interrupt
*
****************************************************************/

void timer_setup() {
    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
    *TIMER1_TOP = SAMPLE_PERIOD;
    *TIMER1_IEN = 1;
}

/***************************************************************
* FUNCTION NAME    timer_disable
*
* DESCRIPTION:
*
*		Stops clock and prevent interrupt 
*
****************************************************************/

void timer_disable() {
    *CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER1;
    *TIMER1_IEN = 0;
}

/***************************************************************
* FUNCTION NAME    timer_start
*
* DESCRIPTION:
*
*		Starts timer by writing 1 to TIMER1_CMD
*
****************************************************************/

void timer_start() {
    *TIMER1_CMD = 1;
}

/***************************************************************
* FUNCTION NAME    timer_stop
*
* DESCRIPTION:
*
*		Stops timer by doing the opposite of timer_start  
*
****************************************************************/

void timer_stop() {
    *TIMER1_CMD = 0;
}


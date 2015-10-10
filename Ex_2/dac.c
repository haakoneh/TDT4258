#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"

/***************************************************************
* FUNCTION NAME    dac_setup
*
* DESCRIPTION:
*
*		Enables dac by setting bit 17 in CMU_HFPERCLKEN0 to 1, 
*		write 1/2^5 to ctrl, enable left an right dac by writing 
*		1 to them 
*
****************************************************************/

void dac_setup() {
    *CMU_HFPERCLKEN0 |= (1 << 17);
    *DAC0_CTRL = 0x50014;
    *DAC0_CH0CTRL = 1;
    *DAC0_CH1CTRL = 1;
}

/***************************************************************
* FUNCTION NAME    dac_disable
*
* DESCRIPTION:
*
*		Does the oposite of dac_setup
*
****************************************************************/

void dac_disable() {
    *DAC0_CTRL = 0;
    *DAC0_CH0CTRL = 0;
    *DAC0_CH1CTRL = 0;
    *CMU_HFPERCLKEN0 &= ~(1 << 17);
}

/***************************************************************
* FUNCTION NAME    dac_write
*
* DESCRIPTION:
*
*		Writes the same data to both channels
*
****************************************************************/

void dac_write(int data){
    *DAC0_CH0DATA = data;
    *DAC0_CH1DATA = data;
}


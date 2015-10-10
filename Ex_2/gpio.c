#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to set up GPIO mode and interrupts*/

/***************************************************************
* FUNCTION NAME    gpio_setup
*
* DESCRIPTION:
*
*		Enable gpio clock, set high drive strength, pins 
*		A8-15 as output, pins C0-7 as input. Enable internal 
*		pull-up, chuse interrupt on falling edge and turn off leds
*
****************************************************************/

void gpio_setup() {
    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO; /* enable GPIO clock*/
    *GPIO_PA_CTRL = 2; /* set high drive strength */
    *GPIO_PA_MODEH = 0x55555555; /* set pins A8-15 as output */
    *GPIO_PC_MODEL = 0x33333333; /* */
    *GPIO_PC_DOUT = 0xFF; /* Enable internal pull-up*/
    *GPIO_EXTIPSELL = 0x22222222;
    *GPIO_EXTIFALL = 0xFF; /* Interrupt on falling edge */
    *GPIO_IEN = 0x00FF;
    *GPIO_IFC = 0xFF;
    *GPIO_PA_DOUT = 0xFF00;
}

/***************************************************************
* FUNCTION NAME    gpio_clear
*
* DESCRIPTION:
*
*		Clears any I/O by coping the GPIO_IF
*
****************************************************************/

void gpio_clear() {
    *GPIO_IFC = *GPIO_IF;
}

/***************************************************************
* FUNCTION NAME    gpio_button_to_led
*
* DESCRIPTION:
*
*		Send button input to leds 
*
****************************************************************/

void gpio_button_to_led() {
    *GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
}

/***************************************************************
* FUNCTION NAME    gpio_led_clear
*
* DESCRIPTION:
*
*		Turn off all led on gamepad 
*
****************************************************************/

void gpio_led_clear(){
    *GPIO_PA_DOUT = 0xffff;
}

/***************************************************************
* FUNCTION NAME    main
*
* DESCRIPTION:
*
*		Maps input to corresponding gampepad button.
*		Returns 0 if no button is pressed, or if multiple 
*		buttons are pressed
*
****************************************************************/

int gpio_map_input() {
    int button_input = ~(*GPIO_PC_DIN);
    int left_shift;
    for(left_shift = 0; left_shift < 8; left_shift++){
        if((1 << left_shift) == (button_input & (1 << left_shift))){
            return (left_shift + 1);
        }
    }
    
    return 0;
}


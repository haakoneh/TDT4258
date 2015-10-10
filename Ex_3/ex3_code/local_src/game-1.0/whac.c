#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <time.h>
#include "display.h"
#include "whac.h"

#define BOX_SIDE 30
#define NUMBER_OF_BUTTONS 8 

#define SW0 0
#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8
#define SW5 16
#define SW6 32
#define SW7 64
#define SW8 128

typedef struct{
	int x, y, side;

}box;

//Array for the boxes and moles

box boxes[NUMBER_OF_BUTTONS];

int active_mole;
int buffer, line, oflags;

color black_color = {0, 0, 0};
color box_color = {31, 25, 31};

/***************************************************************
* FUNCTION NAME    interrupt_handler()
*
* DESCRIPTION:
*
*		Called when input is registered. Read at byte which 
*		gives which buttons are pressed
*
****************************************************************/

void interrupt_handler(int signum){
	read((line), &buffer, 1);
}

/***************************************************************
* FUNCTION NAME    driver_setup()
*
* DESCRIPTION:
*
*		Sets up the char driver by opening it and calling the 
*		function interrupt_handler whenever a signal becomes 
*		available
*
****************************************************************/

void driver_setup(){
	line = open("/dev/driver-gamepad", O_RDONLY);
	signal(SIGIO, &interrupt_handler);
	fcntl(line, F_SETOWN, getpid());
	oflags = fcntl( line, F_GETFL);
	fcntl(line, F_SETFL, oflags | FASYNC);
}

/***************************************************************
* FUNCTION NAME    input_format
*
* DESCRIPTION:
*
*		Checks which box the player whacks 
*
****************************************************************/

void input_format(){
	switch(buffer) {
		case SW0:
			return;
		case SW1:
			whac_compare_buttons_to_moles(0);
			break;
		case SW2:
			whac_compare_buttons_to_moles(1);
			break;			
		case SW3:
			whac_compare_buttons_to_moles(2);
			break;
		case SW4:
			whac_compare_buttons_to_moles(3);
			break;
		case SW5:
			whac_compare_buttons_to_moles(4);
			break;
		case SW6:
			whac_compare_buttons_to_moles(5);
			break;
		case SW7:
			whac_compare_buttons_to_moles(6);
			break;
		case SW8:
			whac_compare_buttons_to_moles(7);
			break;
	}
}

/***************************************************************
* FUNCTION NAME    whac()
*
* DESCRIPTION:
*
*		Main game function. Calls necessary functions
*
****************************************************************/

void whac(){ 
	srand(time(NULL));
	driver_setup();	
	display_init();
	display_fillScreen(black_color);
	whac_init();
    
    while(1){
        if(buffer != 0)
           input_format();
		nanosleep((struct timespec[]){{0, 800000000}}, NULL);
    }
}

/***************************************************************
* FUNCTION NAME    whac_compare_buttons_to_moles(uint8_t input)
*
* DESCRIPTION:
*
*		Checks if whacked box has any moles by checking 
*		if box is active
*
****************************************************************/

void whac_compare_buttons_to_moles(uint8_t input){
    if(input < 0 || input >   NUMBER_OF_BUTTONS - 1)
        return;
    else if(input == active_mole){
        whac_remove_mole(input);
        whac_add_mole(input);
	}
}

/***************************************************************
* FUNCTION NAME    whac_add_mole(int input)
*
* DESCRIPTION:
*
*		Summon a new mole when a previous is whacked
*
****************************************************************/

void whac_add_mole(int input){  
    int pos = input;
    while(pos == input)
        pos = rand() % NUMBER_OF_BUTTONS;
    active_mole = pos;
    display_rectangle(black_color, boxes[pos].x + 5, boxes[pos].y + 5, 
					  BOX_SIDE - 10, BOX_SIDE - 10);
}

/***************************************************************
* FUNCTION NAME    whac_remove_mole(int pos)
*
* DESCRIPTION:
*
*		Removes mole when whacked 
*
****************************************************************/

void whac_remove_mole(int pos){   
    display_rectangle(box_color, boxes[pos].x + 5, boxes[pos].y + 5, 
					  BOX_SIDE - 10, BOX_SIDE - 10);
}

/***************************************************************
* FUNCTION NAME    whac_init()
*
* DESCRIPTION:
*
*		Sets up structs related to game
*
****************************************************************/

void whac_init(){
    boxes[0].x = 15;	boxes[0].y = 20;
    boxes[1].x = 75;	boxes[2].y = 20;
    boxes[2].x = 135;	boxes[1].y = 20;
    boxes[3].x = 195;	boxes[3].y = 20; 
    
    boxes[4].x = 15;	boxes[4].y = 80;
    boxes[5].x = 75;	boxes[5].y = 80;
    boxes[6].x = 135;	boxes[6].y = 80;
    boxes[7].x = 195;	boxes[7].y = 80;  
  	
	int i;
    for(i = 0; i < NUMBER_OF_BUTTONS; i++){
        display_rectangle(box_color, boxes[i].x, boxes[i].y,BOX_SIDE, BOX_SIDE);
    }
    
    active_mole = rand() % NUMBER_OF_BUTTONS;
    display_rectangle(black_color, boxes[active_mole].x + 5, 
					  boxes[active_mole].y + 5,
                      BOX_SIDE - 10, BOX_SIDE - 10);
}


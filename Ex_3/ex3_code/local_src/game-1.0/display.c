#include <stdio.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h> 
#include "display.h"

#define DISPLAY_WIDTH (320)
#define DISPLAY_HEIGHT (240)
#define DISPLAY_SIZE (320*240)
#define BITS_PER_PIXEL (16)

typedef struct{
	color *screen;
	int file_descriptor;
}display;
//struct fb_fix_screeninfo fixed_info;
//struct fb_var_screeninfo var_info;
display my_display;

/***************************************************************
* FUNCTION NAME    display_init(void)
*
* DESCRIPTION:
*
*		Sets up the display by opening framebuffer an mapping
*		an array to hold all pixels 
*
****************************************************************/

void display_init(void){
	my_display.file_descriptor = open("/dev/fb0", O_RDWR);
	if(my_display.file_descriptor == -1){
		printf("Error opening opening frame buffer");
	}	

	my_display.screen = (color*) mmap(	
								NULL,
								DISPLAY_SIZE * BITS_PER_PIXEL / 8,
								PROT_READ | PROT_WRITE,
								MAP_SHARED,
								my_display.file_descriptor,
								0);
			 
	if(my_display.screen == MAP_FAILED){
		printf("Error mapping display to memory");
	}
	
}

/***************************************************************
* FUNCTION NAME    display_fillScreen(color screen_color)
*
* DESCRIPTION:
*
*		Fills entire screen by traversing entire screen array
*
****************************************************************/

void display_fillScreen(color screen_color){
	struct fb_copyarea rect;

	rect.dx = 0;
	rect.dy = 0;
	
	rect.width = DISPLAY_WIDTH;
	rect.height = DISPLAY_HEIGHT;

	int i;
	for(i = 0; i < DISPLAY_SIZE; i++){
		(my_display.screen)[i] = screen_color;	
	}
	ioctl(my_display.file_descriptor, 0x4680, &rect);
}

/***************************************************************
* FUNCTION NAME    display_rectangle(color rect_color, int x, 
*									int y, int width, int height)
*
* DESCRIPTION:
*
*		Draws a rectangle on the screen 
*
****************************************************************/

void display_rectangle(color rect_color, int x, int y, int width, int height){
	struct fb_copyarea rect;
	
	rect.dx = x;
	rect.dy = y;
	
	rect.width = width;
	rect.height = height;
	
	int i, j;
	for(i = y; i < y + height; i++){
		for(j = x; j < x + width; j++){
			(my_display.screen)[i * DISPLAY_WIDTH + j] = rect_color;
		}
	}		
	ioctl(my_display.file_descriptor, 0x4680, &rect);
}


/*
void display_destruct(void){
	munmap(display.screen, DISPLAY_SIZE * BITS_PER_PIXEL);

}
*/

#pragma once
#include <stdint.h>

typedef struct{
	int16_t blue : 5;
	int16_t green : 6;
	int16_t red : 5;
}color;

void display_fillScreen(color screen_color);

void display_rectangle(color rect_color, int x, int y, int width, int height);

void display_destruct(void);

void display_init(void);

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void gpio_setup();

void gpio_clear();

void gpio_button_to_led();

void gpio_led_clear();

int gpio_map_input();

#endif


#ifndef DAC_H
#define DAC_H

#include "stdint.h"

void dac_setup();

void dac_disable(); 

void dac_write(int data);

#endif 


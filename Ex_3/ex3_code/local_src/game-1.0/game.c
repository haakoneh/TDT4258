#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>

#include "display.h"
#include "whac.h"


int main(int argc, char *argv[]) {
    whac();    
    exit(EXIT_SUCCESS);
}

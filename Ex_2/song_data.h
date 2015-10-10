#ifndef SONG_DATA_H
#define SONG_DATA_H

#include <stdint.h>
#include "note_data.h"

typedef struct Song {
    uint8_t length;
    Note* notes[];
} Song;

static Song PEWPEW = { 13, {&D5, &C5, &B4, &A4, &G4, &F4, &E4, &D4, &C4, &B3, &A3, &G3, &F3} };


static Song ONEUP = { 18, {&C4, &E4, &G4, &C4, &E4, &G4, &C5, &E5, &G5, &C5, &E5, &G5, &C6, &E6, &G6, &C7, &E7, &G7} };


static Song JUMP = { 8, {&C5, &D5, &E5, &F5, &G5, &A5, &B5, &C6} };

#endif

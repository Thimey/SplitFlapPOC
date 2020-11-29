#pragma once

#include <Arduino.h>

#define NUMBER_OF_SPLIT_FLAPS (2)
#define MAX_SPLIT_FLAPS (8)
#define NUMBER_OF_FLAPS (40)
#define STEPS_PER_REVOLUTION (200)

const uint8_t flaps[NUMBER_OF_FLAPS] = {
    ' ',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '.',
    ',',
    '\'',
};

const int PULSE_DELAY = 2000;

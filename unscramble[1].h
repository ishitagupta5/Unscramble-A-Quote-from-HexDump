#ifndef UNSCRAMBLE_H
#define UNSCRAMBLE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "word.h"

// Function to unscramble clear data
void clear(uint8_t *encoded_bytes, struct Word_List *decoded_text);
void fuzzy(uint8_t *encoded_bytes, struct Word_List *decoded_text);
#endif
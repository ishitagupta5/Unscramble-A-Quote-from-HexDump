//ishitagupta :)
#include "unscramble.h"
#include "word.h"
#include <stdio.h>
#include <stdlib.h>

// function to decode non-fuzzy data
void clear(uint8_t *encoded_bytes, struct Word_List *decoded_text) {
    // get the number of segments from the encoded data
    uint16_t num_segments = *(uint16_t *)encoded_bytes;
    init_list(decoded_text, num_segments);

    // initialize offset to the start of the segments
    uint16_t segment_offset = *(uint16_t *)(encoded_bytes + 2);

    for (int segmentIndex = 0; segmentIndex < num_segments; segmentIndex++) {
        uint8_t segmentSize = *(encoded_bytes + segment_offset);
        uint16_t next_segment_offset = *(uint16_t *)(encoded_bytes + segment_offset + 1);

        char *decoded_segment = calloc(segmentSize - 2, sizeof(char));
        for (int charIndex = 0; charIndex < segmentSize - 3; charIndex++) {
            *(decoded_segment + charIndex) = *(encoded_bytes + segment_offset + 3 + charIndex);
        }

        struct Word word;
        init_word(&word);
        update_word(&word, decoded_segment, segment_offset + 3);
        add_word(decoded_text, word);
        display_word(stdout, word);

        // move to the next segment
        segment_offset = next_segment_offset;

        free(decoded_segment);
    }
}

// function to decode fuzzy data
void fuzzy(uint8_t *encoded_bytes, struct Word_List *decoded_text) {
    // get the number of segments from the encoded data
    uint16_t num_segments = *(uint16_t *)encoded_bytes;
    init_list(decoded_text, num_segments);

    // initialize offset to the start of the segments
    uint16_t segment_offset = *(uint16_t *)(encoded_bytes + 2);

    for (int i = 0; i < num_segments; i++) {
        uint8_t segment_size = *(encoded_bytes + segment_offset);
        // get the offset of next segment
        uint16_t next_segment_offset = *(uint16_t *)(encoded_bytes + segment_offset + 1);
        uint8_t flipped_length = ((segment_size & 0x0F) << 4) | ((segment_size & 0xF0) >> 4);
        char *decoded_segment = calloc(segment_size - 2, sizeof(char));

        for (int charIndex = 0; charIndex < segment_size - 3; charIndex++) {
            *(decoded_segment + charIndex) = *(encoded_bytes + segment_offset + 3 + charIndex) ^ flipped_length;
        }
        
        char ch = *decoded_segment;
        uint16_t mask = ch;
        mask = (mask << 8) | mask;
        next_segment_offset = next_segment_offset ^ mask;

        struct Word word;
        init_word(&word);
        update_word(&word, decoded_segment, segment_offset + 3);
        add_word(decoded_text, word);
        display_word(stdout, word);
        segment_offset = next_segment_offset;
        free(decoded_segment);
    }
}

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_PLAYERS 3
#define NUM_PILES 3
#define MAX_PILE_SIZE 3
#define NUM_COLORS 7
#define CARDS_PER_COLOR 9
#define COLORS_IN_PLAY 6

typedef enum {
    COLOR_RED = 0,
    COLOR_BLUE,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_ORANGE,
    COLOR_BROWN,
    COLOR_GRAY,
    COLOR_LAST_ROUND
} Color;

#define CHECK_NULL(ptr, msg) \
    if ((ptr) == NULL) { \
        fprintf(stderr, "Error: %s\n", msg); \
        exit(EXIT_FAILURE); \
    }

#define SAFE_FREE(ptr) \
    if ((ptr) != NULL) { \
        free(ptr); \
        (ptr) = NULL; \
    }

const char* color_to_string(Color color);

#endif

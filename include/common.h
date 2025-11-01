#ifndef COMMON_H
#define COMMON_H

// Cabeceras C++
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <algorithm>
#include <iostream>
#include <limits>

// Constantes de juego (mantener compatibilidad)
#define NUM_PLAYERS 3
#define NUM_PILES 3
#define MAX_PILE_SIZE 3
#define NUM_COLORS 7
#define CARDS_PER_COLOR 9
#define COLORS_IN_PLAY 6

// Enumeración de colores (C++)
enum class Color {
    RED = 0,
    BLUE,
    GREEN,
    YELLOW,
    ORANGE,
    BROWN,
    GRAY,
    LAST_ROUND
};

// Utilidades de depuración (podrían no usarse con RAII, pero se conservan)
#define CHECK_NULL(ptr, msg) \
    if ((ptr) == NULL) { \
        std::fprintf(stderr, "Error: %s\n", msg); \
        std::exit(EXIT_FAILURE); \
    }

#define SAFE_FREE(ptr) \
    if ((ptr) != NULL) { \
        free(ptr); \
        (ptr) = NULL; \
    }

// Conversión de color a texto
const char* color_to_string(Color color);

#endif

#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "deck.h"

// ============================================
// CONSTANTES
// ============================================

#define INITIAL_CARD_CAPACITY 15
#define TOP_COLORS_COUNT 3

// ============================================
// ESTRUCTURA DE DATOS
// ============================================

/**
 * Estructura que representa a un jugador
 */
typedef struct {
    char *name;                  // Nombre del jugador (memoria dinámica)
    int id;                      // ID del jugador (0, 1, 2)
    Card **collected_cards;      // Array dinámico de punteros a cartas
    int card_count;              // Número actual de cartas
    int max_cards;               // Capacidad actual del array
    int score;                   // Puntuación final
    bool has_passed;             // true si el jugador ya pasó en esta ronda
} Player;

// ============================================
// FUNCIONES DE CREACIÓN Y DESTRUCCIÓN
// ============================================

/**
 * Crea un nuevo jugador
 */
Player* create_player(const char* name, int id);

/**
 * Libera la memoria de un jugador
 * NOTA: NO libera las cartas individuales, solo el array
 */
void free_player(Player* player);

// ============================================
// FUNCIONES DE GESTIÓN DE CARTAS
// ============================================

/**
 * Añade una carta a la colección del jugador
 * Redimensiona el array automáticamente si es necesario
 */
void add_card_to_collection(Player* player, Card* card);

/**
 * Redimensiona el array de cartas del jugador (uso interno)
 * Duplica la capacidad actual
 */
void resize_card_collection(Player* player);

/**
 * Cuenta cuántas cartas de un color específico tiene el jugador
 */
int count_cards_by_color(Player* player, Color color);

// ============================================
// FUNCIONES DE PUNTUACIÓN
// ============================================

/**
 * Calcula la puntuación final del jugador
 * Implementa las reglas de Coloretto:
 * - Los 3 colores con más cartas dan puntos positivos
 * - Los demás colores dan puntos negativos
 * - Las cartas +2 siempre suman 2 puntos
 */
void calculate_score(Player* player);

/**
 * Obtiene el puntaje positivo según el número de cartas
 */
int get_positive_score(int card_count);

/**
 * Obtiene el puntaje negativo según el número de cartas
 */
int get_negative_score(int card_count);

/**
 * Identifica los 3 colores con más cartas
 */
void find_top_3_colors(int color_counts[], int top_indices[]);

/**
 * Verifica si un color está en el top 3
 */
bool is_in_top3(int color_index, int top_indices[]);

// ============================================
// FUNCIONES DE DISPLAY
// ============================================

/**
 * Imprime la colección de cartas del jugador
 * Agrupa por colores para mejor visualización
 */
void print_player_collection(Player* player);

/**
 * Imprime un resumen del jugador (nombre, cartas, puntuación)
 */
void print_player_summary(Player* player);

/**
 * Imprime el desglose detallado de la puntuación
 * Muestra puntos por cada color y el total
 */
void print_score_breakdown(Player* player);

// ============================================
// FUNCIONES AUXILIARES
// ============================================

/**
 * Resetea el estado del jugador para una nueva ronda
 */
void reset_player_turn(Player* player);

/**
 * Verifica si el jugador ya pasó su turno
 */
bool player_has_passed(Player* player);

#endif // PLAYER_H
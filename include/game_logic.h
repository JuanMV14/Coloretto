#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "common.h"
#include "player.h"
#include "deck.h"

// ============================================
// ESTRUCTURA PRINCIPAL DEL JUEGO
// ============================================

/**
 * Estructura que contiene todo el estado del juego
 */
typedef struct {
    Player **players;             // Array de 3 jugadores
    int num_players;              // Siempre 3
    Deck *main_deck;              // Baraja principal
    Pile **piles;                 // Array de 3 pilas
    int num_piles;                // Siempre 3
    int current_player_idx;       // Índice del jugador actual
    int current_round;            // Número de ronda actual
    bool last_round_triggered;    // true cuando aparece carta "Última Ronda"
    bool game_over;               // true cuando el juego termina
    int players_passed_count;     // Contador de jugadores que pasaron
} Game;

// ============================================
// FUNCIONES DE INICIALIZACIÓN
// ============================================

/**
 * Inicializa el juego completo
 * Crea jugadores, pilas y baraja
 */
Game* init_game(void);

/**
 * Asigna un color inicial a cada jugador
 * Cada jugador recibe 1 carta de un color específico
 */
void assign_initial_colors(Game* game);

/**
 * Prepara el juego para comenzar
 * Baraja el mazo y resetea contadores
 */
void prepare_game(Game* game);

// ============================================
// FUNCIONES DEL CICLO DE JUEGO
// ============================================

/**
 * Verifica si el juego sigue en curso
 */
bool game_is_running(Game* game);

/**
 * Ejecuta el turno completo de un jugador
 */
void player_turn(Game* game);

/**
 * Maneja la acción de robar una carta y colocarla en una pila
 */
bool handle_draw_card_action(Game* game, int pile_index);

/**
 * Maneja la acción de tomar una pila completa
 */
bool handle_take_pile_action(Game* game, int pile_index);

/**
 * Avanza al siguiente jugador
 */
void next_player(Game* game);

/**
 * Verifica si todos los jugadores han pasado
 */
bool all_players_passed(Game* game);

/**
 * Resetea la ronda (limpia pilas y estados de jugadores)
 */
void reset_round(Game* game);

// ============================================
// FUNCIONES DE FINALIZACIÓN
// ============================================

/**
 * Calcula las puntuaciones finales y declara al ganador
 */
void calculate_scores_and_declare_winner(Game* game);

/**
 * Determina el ganador del juego
 */
Player* get_winner(Game* game);

/**
 * Libera TODA la memoria del juego
 * Incluyendo jugadores, cartas, pilas y baraja
 */
void cleanup(Game* game);

// ============================================
// FUNCIONES DE INTERFAZ/DISPLAY
// ============================================

/**
 * Muestra el menú de opciones del jugador
 */
void display_menu(void);

/**
 * Muestra el estado actual del juego
 */
void display_game_state(Game* game);

/**
 * Muestra las pilas disponibles
 */
void display_piles(Game* game);

/**
 * Muestra un mensaje de bienvenida
 */
void display_welcome_message(void);

/**
 * Muestra los resultados finales
 */
void display_final_results(Game* game);

// ============================================
// FUNCIONES DE ENTRADA
// ============================================

/**
 * Obtiene la elección del jugador del menú
 */
int get_player_choice(void);

/**
 * Obtiene el índice de la pila seleccionada
 */
int get_pile_selection(int max_index);

/**
 * Espera a que el usuario presione Enter
 */
void wait_for_enter(void);

/**
 * Limpia el buffer de entrada
 */
void clear_input_buffer(void);

/**
 * Imprime una línea separadora
 */
void print_separator(void);

#endif // GAME_LOGIC_H


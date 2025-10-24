#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "common.h"
#include "player.h"
#include "deck.h"

// ============================================
// ESTRUCTURA PRINCIPAL DEL JUEGO
// ============================================

// Aquí guardo todo lo que necesito para el juego
typedef struct {
    Player **players;             // Los 3 jugadores
    int num_players;              // Cantidad de jugadores (siempre 3)
    Deck *main_deck;              // El mazo de donde sacamos cartas
    Pile **piles;                 // Las 3 pilas donde van las cartas
    int num_piles;                // Cantidad de pilas (siempre 3)
    int current_player_idx;       // Quién está jugando ahora
    int current_round;            // En qué ronda vamos
    bool last_round_triggered;    // Si ya salió la carta de última ronda
    bool game_over;               // Si el juego ya terminó
    int players_passed_count;     // Cuántos jugadores ya pasaron su turno
} Game;

// ============================================
// FUNCIONES PARA EMPEZAR EL JUEGO
// ============================================

// Crea todo lo necesario para empezar: jugadores, pilas y el mazo
Game* init_game(void);

// Le da un color inicial a cada jugador (una carta de cada color)
void assign_initial_colors(Game* game);

// Prepara todo para comenzar a jugar (mezcla el mazo y reinicia los contadores)
void prepare_game(Game* game);

// ============================================
// FUNCIONES MIENTRAS JUGAMOS
// ============================================

// Revisa si el juego todavía no ha terminado
bool game_is_running(Game* game);

// Ejecuta todo lo que pasa en el turno de un jugador
void player_turn(Game* game);

// Cuando el jugador roba una carta y la pone en una pila
bool handle_draw_card_action(Game* game, int pile_index);

// Cuando el jugador decide tomar una pila completa
bool handle_take_pile_action(Game* game, int pile_index);

// Pasa el turno al siguiente jugador
void next_player(Game* game);

// Revisa si todos los jugadores ya pasaron
bool all_players_passed(Game* game);

// Reinicia la ronda (limpia las pilas y resetea a los jugadores)
void reset_round(Game* game);

// ============================================
// FUNCIONES PARA TERMINAR EL JUEGO
// ============================================

// Calcula los puntos de todos y dice quién ganó
void calculate_scores_and_declare_winner(Game* game);

// Encuentra quién es el ganador
Player* get_winner(Game* game);

// Libera toda la memoria que usamos (jugadores, cartas, pilas, mazo)
void cleanup(Game* game);

// ============================================
// FUNCIONES PARA MOSTRAR COSAS EN PANTALLA
// ============================================

// Muestra las opciones que tiene el jugador
void display_menu(void);

// Muestra cómo va el juego (ronda, jugador actual, etc)
void display_game_state(Game* game);

// Muestra las pilas que hay disponibles
void display_piles(Game* game);

// Muestra el mensaje de bienvenida cuando empieza el juego
void display_welcome_message(void);

// Muestra los resultados y el ganador al final
void display_final_results(Game* game);

// ============================================
// FUNCIONES PARA LEER LO QUE ESCRIBE EL USUARIO
// ============================================

// Lee qué opción eligió el jugador del menú
int get_player_choice(void);

// Lee qué pila eligió el jugador
int get_pile_selection(int max_index);

// Espera a que el usuario presione Enter para continuar
void wait_for_enter(void);

// Limpia el buffer del teclado (por si quedó basura)
void clear_input_buffer(void);

// Imprime una línea para separar secciones
void print_separator(void);

#endif // GAME_LOGIC_H


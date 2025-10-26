# Coloretto
Este proyecto consiste en el desarrollo del clásico juego de estrategia Coloreto, implementado completamente en C++, haciendo uso de memoria dinámica para la gestión eficiente de los datos del juego

```mermaid
---
title: Diagrama de Clases UML - Proyecto Coloretto
---
classDiagram
    %% ============================================
    %% ENUMERACION COLOR (STELLA)
    %% ============================================
    class Color {
        <<enumeration>>
        COLOR_RED
        COLOR_BLUE
        COLOR_GREEN
        COLOR_YELLOW
        COLOR_ORANGE
        COLOR_BROWN
        COLOR_GRAY
        COLOR_LAST_ROUND
    }

    %% ============================================
    %% CLASE CARD (STELLA)
    %% ============================================
    class Card {
        -Color color
        -int value
        +create_card(color, value) Card*
        +free_card(card) void
        +print_card(card) void
    }

    %% ============================================
    %% CLASE DECK (STELLA)
    %% ============================================
    class Deck {
        -Card** cards
        -int count
        -int capacity
        -int top_index
        +create_full_deck() Deck*
        +shuffle_deck(deck) void
        +draw_card(deck) Card*
        +deck_is_empty(deck) bool
        +free_deck(deck) void
        +print_deck(deck) void
    }

    %% ============================================
    %% CLASE PILE (STELLA)
    %% ============================================
    class Pile {
        -Card** cards
        -int count
        -bool is_full
        +create_pile() Pile*
        +add_card_to_pile(pile, card) bool
        +pile_is_full(pile) bool
        +pile_is_empty(pile) bool
        +clear_pile(pile) void
        +free_pile(pile) void
        +print_pile(pile, number) void
    }

    %% ============================================
    %% CLASE PLAYER (IVAN)
    %% ============================================
    class Player {
        -char* name
        -int id
        -Card** collected_cards
        -int card_count
        -int max_cards
        -int score
        -bool has_passed
        +create_player(name, id) Player*
        +free_player(player) void
        +add_card_to_collection(player, card) void
        +resize_card_collection(player) void
        +count_cards_by_color(player, color) int
        +calculate_score(player) void
        +get_positive_score(count) int
        +get_negative_score(count) int
        +find_top_3_colors(counts, indices) void
        +print_player_collection(player) void
        +print_score_breakdown(player) void
        +print_player_summary(player) void
        +reset_player_turn(player) void
    }

    %% ============================================
    %% CLASE GAME (JUAN)
    %% ============================================
    class Game {
        -Player** players
        -int num_players
        -Deck* main_deck
        -Pile** piles
        -int num_piles
        -int current_player_idx
        -int current_round
        -bool last_round_triggered
        -bool game_over
        -int players_passed_count
        +init_game() Game*
        +assign_initial_colors(game) void
        +prepare_game(game) void
        +game_is_running(game) bool
        +player_turn(game) void
        +handle_draw_card_action(game, pile_idx) bool
        +handle_take_pile_action(game, pile_idx) bool
        +next_player(game) void
        +all_players_passed(game) bool
        +reset_round(game) void
        +calculate_scores_and_declare_winner(game) void
        +get_winner(game) Player*
        +cleanup(game) void
        +display_game_state(game) void
        +display_piles(game) void
        +display_menu() void
        +display_welcome_message() void
        +display_final_results(game) void
        +get_player_choice() int
        +get_pile_selection(max_index) int
        +wait_for_enter() void
        +clear_input_buffer() void
        +print_separator() void
    }

    %% ============================================
    %% RELACIONES
    %% ============================================
    
    Card --> Color : uses
    Deck o-- Card : contains *
    Pile o-- Card : contains 0..3
    Player o-- Card : collects *
    Game *-- Player : has 3..5
    Game *-- Deck : has 1
    Game *-- Pile : has 3..5

    %% ============================================
    %% NOTAS
    %% ============================================
    
    note for Card "Responsable: Stella\n\nRepresenta una carta individual\ncon un color y valor.\n\nvalue:\n- 1 para colores normales\n- 0 para carta ultima ronda"
    
    note for Deck "Responsable: Stella\n\nBaraja principal del juego.\nUsa algoritmo Fisher-Yates\npara barajar.\n\nTotal: 55 cartas\n- 54 cartas de colores (6x9)\n- 1 carta ultima ronda\n\nUsa malloc para crear el\narray dinamico de cartas."

```
    note for Player "Responsable: Ivan\n\nGestiona la coleccion de cartas\nde cada jugador.\n\nSistema de puntuacion:\n- Top 3 colores: puntos positivos\n- Resto de colores: puntos negativos\n\nUsa realloc para redimensionar\nel array de cartas dinamicamente."
    
    note for Game "Responsable: Juan\n\nContenedor principal que\ncoordina todo el juego.\n\nSoporta 3-5 jugadores (variable).\nEl numero de pilas es igual al\nnumero de jugadores.\n\nControla:\n- Ciclo de turnos\n- Estado del juego\n- Interfaz de usuario\n- Limpieza de memoria\n\nUsa malloc para crear arrays\ndinamicos de jugadores y pilas."
    
    note for Pile "Responsable: Stella\n\nMaximo 3 cartas por pila.\nNumero de pilas en mesa: 3-5\n(igual al numero de jugadores)."

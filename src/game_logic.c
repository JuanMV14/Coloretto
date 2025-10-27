#include "../include/game_logic.h"

// ============================================
// FUNCIONES PARA EMPEZAR EL JUEGO
// ============================================

Game* init_game(void) {
    Game* game = (Game*)malloc(sizeof(Game));
    CHECK_NULL(game, "No se pudo crear el juego");
    
    // Preguntar cuántos jugadores van a jugar
    printf("\nCuantos jugadores van a jugar? (3-5): ");
    int num_players = 0;
    while (num_players < 3 || num_players > 5) {
        if (scanf("%d", &num_players) != 1) {
            clear_input_buffer();
            printf("Entrada invalida. Intenta de nuevo (3-5): ");
            continue;
        }
        clear_input_buffer();
        
        if (num_players < 3 || num_players > 5) {
            printf("Numero invalido. Debe ser entre 3 y 5: ");
        }
    }
    
    // Inicializar valores básicos
    game->num_players = num_players;
    game->num_piles = num_players;  // Mismo número de pilas que jugadores
    game->current_player_idx = 0;
    game->current_round = 1;
    game->last_round_triggered = false;
    game->game_over = false;
    game->players_passed_count = 0;
    
    // Crear array de jugadores
    game->players = (Player**)malloc(sizeof(Player*) * num_players);
    CHECK_NULL(game->players, "No se pudo crear array de jugadores");
    
    // Pedir nombres de los jugadores
    char name_buffer[50];
    for (int i = 0; i < num_players; i++) {
        printf("Nombre del jugador %d: ", i + 1);
        if (scanf("%49s", name_buffer) == 1) {
            clear_input_buffer();
            game->players[i] = create_player(name_buffer, i);
        }
    }
    
    // Crear array de pilas
    game->piles = (Pile**)malloc(sizeof(Pile*) * num_players);
    CHECK_NULL(game->piles, "No se pudo crear array de pilas");
    
    for (int i = 0; i < game->num_piles; i++) {
        game->piles[i] = create_pile();
    }
    
    // La baraja se crea externamente en main
    game->main_deck = NULL;
    
    printf("\nJuego inicializado para %d jugadores\n", num_players);
    
    return game;
}

void assign_initial_colors(Game* game) {
    if (game == NULL || game->main_deck == NULL) return;
    
    printf("\nAsignando colores iniciales...\n");
    
    for (int i = 0; i < game->num_players; i++) {
        Card* initial_card = draw_card(game->main_deck);
        if (initial_card != NULL) {
            add_card_to_collection(game->players[i], initial_card);
            printf("  %s recibe: %s\n", 
                   game->players[i]->name, 
                   color_to_string(initial_card->color));
        }
    }
}

void prepare_game(Game* game) {
    if (game == NULL) return;
    
    printf("\nPreparando el juego...\n");
    shuffle_deck(game->main_deck);
    
    printf("Juego listo para comenzar\n");
    wait_for_enter();
}

// ============================================
// FUNCIONES MIENTRAS JUGAMOS
// ============================================

bool game_is_running(Game* game) {
    if (game == NULL) return false;
    
    // Si ya se activó game_over
    if (game->game_over) return false;
    
    // Si la baraja está vacía
    if (deck_is_empty(game->main_deck)) {
        game->game_over = true;
        return false;
    }
    
    return true;
}

void player_turn(Game* game) {
    if (game == NULL) return;
    
    Player* current = game->players[game->current_player_idx];
    
    // Si el jugador ya pasó, saltar
    if (current->has_passed) {
        next_player(game);
        return;
    }
    
    display_game_state(game);
    
    printf("\n=======================================\n");
    printf("   TURNO DE: %s\n", current->name);
    printf("=======================================\n");
    
    display_menu();
    
    int choice = get_player_choice();
    
    if (choice == 1) {
        // Robar carta del mazo
        Card* drawn = draw_card(game->main_deck);
        if (drawn == NULL) {
            printf("\nNo hay mas cartas en la baraja\n");
            return;
        }
        
        printf("\nCarta robada: %s\n", color_to_string(drawn->color));
        
        // Verificar si es "Ultima Ronda"
        if (drawn->color == COLOR_LAST_ROUND) {
            printf("\n");
            printf("=======================================\n");
            printf("   ULTIMA RONDA ACTIVADA!\n");
            printf("=======================================\n");
            game->last_round_triggered = true;
            free_card(drawn);
            
            // Robar otra carta
            drawn = draw_card(game->main_deck);
            if (drawn == NULL) {
                printf("No hay mas cartas en la baraja\n");
                return;
            }
            printf("Nueva carta: %s\n", color_to_string(drawn->color));
        }
        
        // Ahora preguntar donde colocarla
        printf("\nEn que pila deseas colocar la carta? (1-%d): ", game->num_piles);
        int pile_idx = get_pile_selection(game->num_piles) - 1;
        
        // Verificar si la pila ya fue tomada
        if (game->piles[pile_idx]->is_taken) {
            printf("Esa pila ya fue tomada en esta ronda\n");
            free_card(drawn);
            return;
        }
        
        // Verificar si la pila esta llena
        if (pile_is_full(game->piles[pile_idx])) {
            printf("La pila esta llena\n");
            free_card(drawn);
            return;
        }
        
        // Colocar la carta
        if (add_card_to_pile(game->piles[pile_idx], drawn)) {
            printf("Carta colocada en la pila %d\n", pile_idx + 1);
        } else {
            printf("No se pudo colocar la carta\n");
            free_card(drawn);
        }
        
    } else if (choice == 2) {
        // Tomar una pila
        display_piles(game);
        printf("\nQue pila deseas tomar? (1-%d): ", game->num_piles);
        int pile_idx = get_pile_selection(game->num_piles) - 1;
        
        if (handle_take_pile_action(game, pile_idx)) {
            printf("%s tomo la pila %d\n", current->name, pile_idx + 1);
            current->has_passed = true;
            game->players_passed_count++;
        } else {
            printf("No se pudo tomar la pila\n");
        }
    }
    
    // Verificar si todos pasaron
    if (all_players_passed(game)) {
        printf("\nTodos los jugadores han pasado. Nueva ronda.\n");
        reset_round(game);
        wait_for_enter();
    }
    
    next_player(game);
    wait_for_enter();
}

bool handle_draw_card_action(Game* game, int pile_index) {
    if (game == NULL) return false;
    if (pile_index < 0 || pile_index >= game->num_piles) return false;
    
    Pile* target_pile = game->piles[pile_index];
    
    // Verificar si la pila está llena
    if (pile_is_full(target_pile)) {
        printf("La pila esta llena\n");
        return false;
    }
    
    // Robar carta
    Card* drawn = draw_card(game->main_deck);
    if (drawn == NULL) {
        printf("No hay mas cartas en la baraja\n");
        return false;
    }
    
    printf("Carta robada: %s\n", color_to_string(drawn->color));
    
    // Verificar si es "Última Ronda"
    if (drawn->color == COLOR_LAST_ROUND) {
        printf("\n");
        printf("=======================================\n");
        printf("   ULTIMA RONDA ACTIVADA!\n");
        printf("=======================================\n");
        game->last_round_triggered = true;
        free_card(drawn);  // Esta carta no se usa
        
        // Robar otra carta
        drawn = draw_card(game->main_deck);
        if (drawn == NULL) return false;
        printf("Nueva carta: %s\n", color_to_string(drawn->color));
    }
    
    // Añadir a la pila
    return add_card_to_pile(target_pile, drawn);
}

bool handle_take_pile_action(Game* game, int pile_index) {
    if (game == NULL) return false;
    if (pile_index < 0 || pile_index >= game->num_piles) return false;
    
    Pile* pile = game->piles[pile_index];
    Player* current = game->players[game->current_player_idx];
    
    // Verificar que la pila no esté vacía
    if (pile_is_empty(pile)) {
        printf("La pila esta vacia\n");
        return false;
    }
    
    // Verificar que la pila no haya sido tomada ya
    if (pile->is_taken) {
        printf("Esa pila ya fue tomada en esta ronda\n");
        return false;
    }
    
    // Transferir cartas
    for (int i = 0; i < pile->count; i++) {
        add_card_to_collection(current, pile->cards[i]);
    }
    
    printf("%s recibio %d carta(s)\n", current->name, pile->count);
    
    // Marcar la pila como tomada (pero no limpiarla aun)
    pile->is_taken = true;
    pile->count = 0;  // Vaciar las cartas pero mantener el flag
    
    return true;
}

void next_player(Game* game) {
    if (game == NULL) return;
    
    int attempts = 0;
    do {
        game->current_player_idx = (game->current_player_idx + 1) % game->num_players;
        attempts++;
        
        // Evitar bucle infinito
        if (attempts > game->num_players) break;
        
    } while (game->players[game->current_player_idx]->has_passed);
}

bool all_players_passed(Game* game) {
    if (game == NULL) return false;
    return game->players_passed_count >= game->num_players;
}

void reset_round(Game* game) {
    if (game == NULL) return;
    
    // Limpiar todas las pilas
    for (int i = 0; i < game->num_piles; i++) {
        clear_pile(game->piles[i]);
    }
    
    // Resetear estado de jugadores
    for (int i = 0; i < game->num_players; i++) {
        reset_player_turn(game->players[i]);
    }
    
    game->players_passed_count = 0;
    game->current_round++;
    
    // Si se activó última ronda, terminar el juego
    if (game->last_round_triggered) {
        printf("\nUltima ronda completada. Fin del juego.\n");
        game->game_over = true;
    }
}

// ============================================
// FUNCIONES PARA TERMINAR EL JUEGO
// ============================================

void calculate_scores_and_declare_winner(Game* game) {
    if (game == NULL) return;
    
    printf("\n");
    printf("=======================================\n");
    printf("     CALCULANDO PUNTUACIONES\n");
    printf("=======================================\n\n");
    
    // Calcular puntuación de cada jugador
    for (int i = 0; i < game->num_players; i++) {
        calculate_score(game->players[i]);
        print_score_breakdown(game->players[i]);
        wait_for_enter();
    }
    
    // Declarar ganador
    Player* winner = get_winner(game);
    
    display_final_results(game);
    
    if (winner != NULL) {
        printf("\n");
        printf("=======================================\n");
        printf("\n");
        printf("   GANADOR: %s\n", winner->name);
        printf("   Puntuacion: %d\n", winner->score);
        printf("\n");
        printf("=======================================\n");
    }
}

Player* get_winner(Game* game) {
    if (game == NULL || game->num_players == 0) return NULL;
    
    Player* winner = game->players[0];
    
    for (int i = 1; i < game->num_players; i++) {
        if (game->players[i]->score > winner->score) {
            winner = game->players[i];
        }
    }
    
    return winner;
}

void cleanup(Game* game) {
    if (game == NULL) return;
    
    printf("\nLiberando memoria...\n");
    
    // Liberar baraja (y todas las cartas)
    if (game->main_deck != NULL) {
        free_deck(game->main_deck);
    }
    
    // Liberar pilas
    if (game->piles != NULL) {
        for (int i = 0; i < game->num_piles; i++) {
            free_pile(game->piles[i]);
        }
        SAFE_FREE(game->piles);
    }
    
    // Liberar jugadores
    if (game->players != NULL) {
        for (int i = 0; i < game->num_players; i++) {
            free_player(game->players[i]);
        }
        SAFE_FREE(game->players);
    }
    
    // Liberar estructura del juego
    SAFE_FREE(game);
    
    printf("Memoria liberada exitosamente\n");
}

// ============================================
// FUNCIONES PARA MOSTRAR COSAS EN PANTALLA
// ============================================

void display_menu(void) {
    printf("\n---------------------------------------\n");
    printf("            TUS OPCIONES               \n");
    printf("---------------------------------------\n");
    printf("  1. Robar carta y colocar en pila    \n");
    printf("  2. Tomar una pila completa           \n");
    printf("---------------------------------------\n");
}

void display_game_state(Game* game) {
    if (game == NULL) return;
    
    printf("\n\n");
    printf("=======================================\n");
    printf("         ESTADO DEL JUEGO              \n");
    printf("=======================================\n");
    printf("  Ronda: %d\n", game->current_round);
    printf("  Cartas restantes: %d\n", 
           game->main_deck->count - game->main_deck->top_index);
    
    if (game->last_round_triggered) {
        printf("  [!] ULTIMA RONDA ACTIVA\n");
    }
    
    printf("=======================================\n\n");
    
    display_piles(game);
    
    printf("\nESTADO DE JUGADORES:\n");
    printf("-----------------------------------------\n");
    for (int i = 0; i < game->num_players; i++) {
        Player* p = game->players[i];
        printf("  %s: %d cartas", p->name, p->card_count);
        if (p->has_passed) {
            printf(" [PASO]");
        }
        if (i == game->current_player_idx) {
            printf(" <- TURNO ACTUAL");
        }
        printf("\n");
    }
    printf("-----------------------------------------\n");
}

void display_piles(Game* game) {
    if (game == NULL) return;
    
    printf("\nPILAS DISPONIBLES:\n");
    printf("-----------------------------------------\n");
    for (int i = 0; i < game->num_piles; i++) {
        print_pile(game->piles[i], i + 1);
    }
    printf("-----------------------------------------\n");
}

void display_welcome_message(void) {
    printf("\n");
    printf("=======================================\n");
    printf("\n");
    printf("        COLORETTO\n");
    printf("\n");
    printf("   Implementacion en C\n");
    printf("\n");
    printf("=======================================\n");
    printf("\n");
}

void display_final_results(Game* game) {
    if (game == NULL) return;
    
    printf("\n");
    printf("=======================================\n");
    printf("        RESULTADOS FINALES\n");
    printf("=======================================\n\n");
    
    // Crear array para ordenar
    Player** sorted = (Player**)malloc(sizeof(Player*) * game->num_players);
    for (int i = 0; i < game->num_players; i++) {
        sorted[i] = game->players[i];
    }
    
    // Ordenamiento burbuja (de mayor a menor score)
    for (int i = 0; i < game->num_players - 1; i++) {
        for (int j = 0; j < game->num_players - i - 1; j++) {
            if (sorted[j]->score < sorted[j + 1]->score) {
                Player* temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }
    
    // Mostrar resultados ordenados
    for (int i = 0; i < game->num_players; i++) {
        printf("%d. ", i + 1);
        print_player_summary(sorted[i]);
    }
    printf("\n");
    
    SAFE_FREE(sorted);
}

// ============================================
// FUNCIONES PARA LEER LO QUE ESCRIBE EL USUARIO
// ============================================

int get_player_choice(void) {
    int choice = 0;
    
    while (choice != 1 && choice != 2) {
        printf("\nTu eleccion (1-2): ");
        fflush(stdout);  // Asegurar que el prompt se muestre
        
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("Entrada invalida. Intenta de nuevo.\n");
            continue;
        }
        
        clear_input_buffer();
        
        if (choice != 1 && choice != 2) {
            printf("Opcion invalida. Elige 1 o 2.\n");
        }
    }
    
    return choice;
}

int get_pile_selection(int max_index) {
    int selection = 0;
    
    fflush(stdout);  // Asegurar que todo se muestre antes de leer
    
    while (selection < 1 || selection > max_index) {
        if (scanf("%d", &selection) != 1) {
            clear_input_buffer();
            printf("Entrada invalida. Intenta de nuevo: ");
            fflush(stdout);
            continue;
        }
        
        clear_input_buffer();
        
        if (selection < 1 || selection > max_index) {
            printf("Seleccion invalida. Elige entre 1 y %d: ", max_index);
            fflush(stdout);
        }
    }
    
    return selection;
}

void wait_for_enter(void) {
    printf("\n[Presiona ENTER para continuar]");
    clear_input_buffer();
    getchar();
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_separator(void) {
    printf("=======================================\n");
}


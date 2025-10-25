#include "../include/game_logic.h"

int main(void) {
    // Mostrar mensaje de bienvenida
    display_welcome_message();
    
    printf("Inicializando juego...\n\n");
    
    // Paso 1: Crear el juego (jugadores, pilas, etc)
    Game* game = init_game();
    
    // Paso 2: Crear y mezclar la baraja
    game->main_deck = create_full_deck();
    shuffle_deck(game->main_deck);
    
    // Paso 3: Dar un color inicial a cada jugador
    assign_initial_colors(game);
    
    // Paso 4: Preparar todo para empezar
    prepare_game(game);
    
    printf("\n");
    printf("=======================================\n");
    printf("        COMIENZA EL JUEGO!\n");
    printf("=======================================\n");
    
    // Paso 5: Aqui empieza el juego (bucle principal)
    while (game_is_running(game)) {
        player_turn(game);
    }
    
    // Paso 6: El juego termino, calcular quien gano
    calculate_scores_and_declare_winner(game);
    
    // Paso 7: Liberar toda la memoria que usamos
    cleanup(game);
    
    printf("\n");
    printf("=======================================\n");
    printf("     GRACIAS POR JUGAR!\n");
    printf("=======================================\n\n");
    
    return 0;
}


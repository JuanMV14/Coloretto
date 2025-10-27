#include "../include/player.h"

// ============================================
// FUNCIONES DE CREACIÓN Y DESTRUCCIÓN
// ============================================

Player* create_player(const char* name, int id) {
    if (name == NULL) {
        fprintf(stderr, "Error: nombre nulo\n");
        return NULL;
    }
    
    Player* player = (Player*)malloc(sizeof(Player));
    CHECK_NULL(player, "No se pudo crear el jugador");
    
    // Asignar nombre (copia dinámica)
    player->name = (char*)malloc(strlen(name) + 1);
    CHECK_NULL(player->name, "No se pudo asignar memoria para el nombre");
    strcpy(player->name, name);
    
    player->id = id;
    player->card_count = 0;
    player->max_cards = INITIAL_CARD_CAPACITY;
    player->score = 0;
    playe…
[10:02 a. m., 23/10/2025] +57 313 5199762: RESUMEN IVÁN
Total de archivos: 2
Total de líneas: ~400 líneas
Complejidad: Media-Alta
Responsabilidades:

Estructura Player completa
Gestión dinámica de cartas con realloc
Sistema de puntuación completo (reglas de Coloretto)
Algoritmo para encontrar top 3 colores
Funciones de display detallado
[10:02 a. m., 23/10/2025] +57 313 5199762: Testing: gcc -Wall -Wextra -g -Iinclude -c src/player.c -o obj/player.o
[10:02 a. m., 23/10/2025] +57 313 5199762: Prueba unitaria:
[10:02 a. m., 23/10/2025] +57 313 5199762: // test_player.c
#include "include/player.h"

int main(void) {
    Player* p = create_player("Test", 0);
    
    // Añadir cartas de prueba
    for(int i = 0; i < 5; i++) {
        Card* c = create_card(COLOR_RED, 1);
        add_card_to_collection(p, c);
    }
    
    calculate_score(p);
    print_score_breakdown(p);
    
    // Liberar
    for(int i = 0; i < p->card_count; i++) {
        free_card(p->collected_cards[i]);
    }
    free_player(p);
    
    return 0;
}
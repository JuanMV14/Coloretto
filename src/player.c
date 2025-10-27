#include "../include/player.h"

// ============================================
// FUNCIONES DE CREACION Y DESTRUCCION
// ============================================

Player* create_player(const char* name, int id) {
    if (name == NULL) {
        fprintf(stderr, "Error: nombre nulo\n");
        return NULL;
    }
    
    Player* player = (Player*)malloc(sizeof(Player));
    CHECK_NULL(player, "No se pudo crear el jugador");
    
    // Asignar nombre (copia dinamica)
    player->name = (char*)malloc(strlen(name) + 1);
    CHECK_NULL(player->name, "No se pudo asignar memoria para el nombre");
    strcpy(player->name, name);
    
    player->id = id;
    player->card_count = 0;
    player->max_cards = INITIAL_CARD_CAPACITY;
    player->score = 0;
    player->has_passed = false;
    
    // Crear array dinamico de cartas
    player->collected_cards = (Card**)malloc(sizeof(Card*) * player->max_cards);
    CHECK_NULL(player->collected_cards, "No se pudo asignar memoria para las cartas");
    
    return player;
}

void free_player(Player* player) {
    if (player == NULL) return;
    
    // Liberar el nombre
    SAFE_FREE(player->name);
    
    // Liberar las cartas individuales
    for (int i = 0; i < player->card_count; i++) {
        free_card(player->collected_cards[i]);
    }
    
    // Liberar el array de cartas
    SAFE_FREE(player->collected_cards);
    
    // Liberar la estructura del jugador
    SAFE_FREE(player);
}

// ============================================
// FUNCIONES DE GESTION DE CARTAS
// ============================================

void add_card_to_collection(Player* player, Card* card) {
    if (player == NULL || card == NULL) return;
    
    // Redimensionar si es necesario
    if (player->card_count >= player->max_cards) {
        resize_card_collection(player);
    }
    
    // Agregar la carta
    player->collected_cards[player->card_count] = card;
    player->card_count++;
}

void resize_card_collection(Player* player) {
    if (player == NULL) return;
    
    int new_capacity = player->max_cards * 2;
    Card** new_array = (Card**)realloc(player->collected_cards, 
                                       sizeof(Card*) * new_capacity);
    CHECK_NULL(new_array, "No se pudo redimensionar el array de cartas");
    
    player->collected_cards = new_array;
    player->max_cards = new_capacity;
}

int count_cards_by_color(Player* player, Color color) {
    if (player == NULL) return 0;
    
    int count = 0;
    for (int i = 0; i < player->card_count; i++) {
        if (player->collected_cards[i]->color == color) {
            count++;
        }
    }
    
    return count;
}

// ============================================
// FUNCIONES DE PUNTUACION
// ============================================

void calculate_score(Player* player) {
    if (player == NULL) return;
    
    // Contar cartas por color
    int color_counts[NUM_COLORS];
    for (int i = 0; i < NUM_COLORS; i++) {
        color_counts[i] = count_cards_by_color(player, i);
    }
    
    // Encontrar los 3 colores con mas cartas
    int top_indices[TOP_COLORS_COUNT];
    find_top_3_colors(color_counts, top_indices);
    
    // Calcular puntuacion
    player->score = 0;
    
    for (int color = 0; color < NUM_COLORS; color++) {
        int count = color_counts[color];
        if (count == 0) continue;
        
        // Ignorar carta de ultima ronda
        if (color == COLOR_LAST_ROUND) continue;
        
        // Verificar si es un color top 3
        if (is_in_top3(color, top_indices)) {
            player->score += get_positive_score(count);
        } else {
            player->score += get_negative_score(count);
        }
    }
}

int get_positive_score(int card_count) {
    switch (card_count) {
        case 1: return 1;
        case 2: return 3;
        case 3: return 6;
        case 4: return 10;
        case 5: return 15;
        default: return 21;  // 6 o mas
    }
}

int get_negative_score(int card_count) {
    switch (card_count) {
        case 1: return -1;
        case 2: return -3;
        case 3: return -6;
        default: return -10;  // 4 o mas
    }
}

void find_top_3_colors(int color_counts[], int top_indices[]) {
    // Inicializar top_indices
    for (int i = 0; i < TOP_COLORS_COUNT; i++) {
        top_indices[i] = -1;
    }
    
    // Encontrar los 3 colores con mas cartas
    for (int i = 0; i < TOP_COLORS_COUNT; i++) {
        int max_count = -1;
        int max_index = -1;
        
        for (int color = 0; color < NUM_COLORS; color++) {
            // Ignorar carta de ultima ronda
            if (color == COLOR_LAST_ROUND) continue;
            
            // Verificar si ya esta en el top
            if (is_in_top3(color, top_indices)) continue;
            
            if (color_counts[color] > max_count) {
                max_count = color_counts[color];
                max_index = color;
            }
        }
        
        if (max_index != -1 && max_count > 0) {
            top_indices[i] = max_index;
        }
    }
}

bool is_in_top3(int color_index, int top_indices[]) {
    for (int i = 0; i < TOP_COLORS_COUNT; i++) {
        if (top_indices[i] == color_index) {
            return true;
        }
    }
    return false;
}

// ============================================
// FUNCIONES DE DISPLAY
// ============================================

void print_player_collection(Player* player) {
    if (player == NULL) return;
    
    printf("\n---------------------------------------\n");
    printf("Coleccion de %s:\n", player->name);
    printf("---------------------------------------\n");
    
    if (player->card_count == 0) {
        printf("  (sin cartas)\n");
    } else {
        // Contar cartas por color
        for (int color = 0; color < NUM_COLORS; color++) {
            int count = count_cards_by_color(player, color);
            if (count > 0) {
                printf("  %s: %d carta(s)\n", 
                       color_to_string(color), count);
            }
        }
    }
    
    printf("---------------------------------------\n");
}

void print_player_summary(Player* player) {
    if (player == NULL) return;
    
    printf("%-15s | %2d cartas | %4d puntos\n", 
           player->name, 
           player->card_count, 
           player->score);
}

void print_score_breakdown(Player* player) {
    if (player == NULL) return;
    
    printf("\n=======================================\n");
    printf("  Desglose de Puntuacion - %s\n", player->name);
    printf("=======================================\n");
    
    // Contar cartas por color
    int color_counts[NUM_COLORS];
    for (int i = 0; i < NUM_COLORS; i++) {
        color_counts[i] = count_cards_by_color(player, i);
    }
    
    // Encontrar top 3
    int top_indices[TOP_COLORS_COUNT];
    find_top_3_colors(color_counts, top_indices);
    
    printf("\nColores principales (puntos positivos):\n");
    printf("---------------------------------------\n");
    for (int i = 0; i < TOP_COLORS_COUNT; i++) {
        if (top_indices[i] != -1) {
            int count = color_counts[top_indices[i]];
            int score = get_positive_score(count);
            printf("  %s: %d carta(s) = +%d puntos\n", 
                   color_to_string(top_indices[i]), 
                   count, 
                   score);
        }
    }
    
    printf("\nOtros colores (puntos negativos):\n");
    printf("---------------------------------------\n");
    bool has_negative = false;
    for (int color = 0; color < NUM_COLORS; color++) {
        if (color == COLOR_LAST_ROUND) continue;
        if (color_counts[color] == 0) continue;
        if (is_in_top3(color, top_indices)) continue;
        
        int count = color_counts[color];
        int score = get_negative_score(count);
        printf("  %s: %d carta(s) = %d puntos\n", 
               color_to_string(color), 
               count, 
               score);
        has_negative = true;
    }
    
    if (!has_negative) {
        printf("  (ninguno)\n");
    }
    
    printf("\n---------------------------------------\n");
    printf("  PUNTUACION TOTAL: %d\n", player->score);
    printf("=======================================\n\n");
}

// ============================================
// FUNCIONES AUXILIARES
// ============================================

void reset_player_turn(Player* player) {
    if (player == NULL) return;
    player->has_passed = false;
}

bool player_has_passed(Player* player) {
    if (player == NULL) return false;
    return player->has_passed;
}


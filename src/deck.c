#include "../include/deck.h"

const char* color_to_string(Color color) {
    switch(color) {
        case COLOR_RED:         return "Rojo";
        case COLOR_BLUE:        return "Azul";
        case COLOR_GREEN:       return "Verde";
        case COLOR_YELLOW:      return "Amarillo";
        case COLOR_ORANGE:      return "Naranja";
        case COLOR_BROWN:       return "Marron";
        case COLOR_GRAY:        return "Gris";
        case COLOR_LAST_ROUND:  return "Ultima Ronda";
        default:                return "Desconocido";
    }
}

Card* create_card(Color color, int value) {
    Card* card = (Card*)malloc(sizeof(Card));
    CHECK_NULL(card, "No se pudo crear la carta");
    
    card->color = color;
    card->value = value;
    
    return card;
}

void free_card(Card* card) {
    SAFE_FREE(card);
}

void print_card(Card* card) {
    if (card == NULL) {
        printf("[Vacio]");
        return;
    }
    printf("[%s]", color_to_string(card->color));
}

Deck* create_full_deck(void) {
    Deck* deck = (Deck*)malloc(sizeof(Deck));
    CHECK_NULL(deck, "No se pudo crear la baraja");
    
    deck->capacity = 70;
    deck->cards = (Card**)malloc(sizeof(Card*) * deck->capacity);
    CHECK_NULL(deck->cards, "No se pudo asignar memoria para las cartas");
    
    deck->count = 0;
    deck->top_index = 0;
    
    srand(time(NULL));
    Color removed_color = (Color)(rand() % NUM_COLORS);
    
    printf("Color removido para esta partida: %s\n", color_to_string(removed_color));
    
    for (Color color = 0; color < NUM_COLORS; color++) {
        if (color == removed_color) continue;
        if (color == COLOR_LAST_ROUND) continue;
        
        for (int i = 0; i < CARDS_PER_COLOR; i++) {
            deck->cards[deck->count++] = create_card(color, 1);
        }
    }
    
    deck->cards[deck->count++] = create_card(COLOR_LAST_ROUND, 0);
    
    printf("Baraja creada: %d cartas totales\n", deck->count);
    
    return deck;
}

void shuffle_deck(Deck* deck) {
    if (deck == NULL || deck->count == 0) return;
    
    for (int i = deck->count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card* temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
    
    int last_round_idx = -1;
    for (int i = 0; i < deck->count; i++) {
        if (deck->cards[i]->color == COLOR_LAST_ROUND) {
            last_round_idx = i;
            break;
        }
    }
    
    if (last_round_idx != -1) {
        int target_position = deck->count - 16;
        if (target_position < 0) target_position = 0;
        if (target_position >= deck->count) target_position = deck->count - 1;
        
        Card* temp = deck->cards[last_round_idx];
        deck->cards[last_round_idx] = deck->cards[target_position];
        deck->cards[target_position] = temp;
        
        printf("Carta Ultima Ronda colocada en posicion %d (desde arriba: %d desde abajo)\n", 
               target_position, deck->count - target_position);
    }
    
    printf("Baraja barajada exitosamente\n");
}

Card* draw_card(Deck* deck) {
    if (deck == NULL || deck_is_empty(deck)) {
        return NULL;
    }
    
    Card* drawn = deck->cards[deck->top_index];
    deck->top_index++;
    
    return drawn;
}

bool deck_is_empty(Deck* deck) {
    if (deck == NULL) return true;
    return deck->top_index >= deck->count;
}

void free_deck(Deck* deck) {
    if (deck == NULL) return;
    
    for (int i = 0; i < deck->count; i++) {
        free_card(deck->cards[i]);
    }
    
    SAFE_FREE(deck->cards);
    SAFE_FREE(deck);
}

void print_deck(Deck* deck) {
    if (deck == NULL) {
        printf("Baraja: NULL\n");
        return;
    }
    
    printf("Baraja: %d cartas (siguiente: %d)\n", 
           deck->count - deck->top_index, deck->top_index);
}

Pile* create_pile(void) {
    Pile* pile = (Pile*)malloc(sizeof(Pile));
    CHECK_NULL(pile, "No se pudo crear la pila");
    
    pile->cards = (Card**)malloc(sizeof(Card*) * MAX_PILE_SIZE);
    CHECK_NULL(pile->cards, "No se pudo asignar memoria para cartas de la pila");
    
    pile->count = 0;
    pile->is_full = false;
    pile->is_taken = false;
    
    return pile;
}

bool add_card_to_pile(Pile* pile, Card* card) {
    if (pile == NULL || card == NULL) return false;
    
    if (pile->count >= MAX_PILE_SIZE) {
        pile->is_full = true;
        return false;
    }
    
    pile->cards[pile->count++] = card;
    
    if (pile->count == MAX_PILE_SIZE) {
        pile->is_full = true;
    }
    
    return true;
}

bool pile_is_full(Pile* pile) {
    if (pile == NULL) return false;
    return pile->is_full;
}

bool pile_is_empty(Pile* pile) {
    if (pile == NULL) return true;
    return pile->count == 0;
}

void clear_pile(Pile* pile) {
    if (pile == NULL) return;
    
    pile->count = 0;
    pile->is_full = false;
    pile->is_taken = false;
}

void free_pile(Pile* pile) {
    if (pile == NULL) return;
    
    SAFE_FREE(pile->cards);
    SAFE_FREE(pile);
}

void print_pile(Pile* pile, int pile_number) {
    if (pile == NULL) {
        printf("Pila %d: NULL\n", pile_number);
        return;
    }
    
    printf("Pila %d [%d/%d]: ", pile_number, pile->count, MAX_PILE_SIZE);
    
    if (pile->is_taken) {
        printf("[TOMADA]");
    } else if (pile->count == 0) {
        printf("Vacia");
    } else {
        for (int i = 0; i < pile->count; i++) {
            print_card(pile->cards[i]);
            if (i < pile->count - 1) printf(" ");
        }
    }
    
    if (pile->is_full) {
        printf(" [LLENA]");
    }
    
    printf("\n");
}

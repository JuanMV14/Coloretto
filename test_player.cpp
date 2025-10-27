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
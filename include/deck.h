#ifndef DECK_H
#define DECK_H

#include "common.h"

typedef struct {
    Color color;
    int value;
} Card;

typedef struct {
    Card **cards;
    int count;
    int capacity;
    int top_index;
} Deck;

typedef struct {
    Card **cards;
    int count;
    bool is_full;
} Pile;

Card* create_card(Color color, int value);
void free_card(Card* card);
void print_card(Card* card);

Deck* create_full_deck(void);
void shuffle_deck(Deck* deck);
Card* draw_card(Deck* deck);
bool deck_is_empty(Deck* deck);
void free_deck(Deck* deck);
void print_deck(Deck* deck);

Pile* create_pile(void);
bool add_card_to_pile(Pile* pile, Card* card);
bool pile_is_full(Pile* pile);
bool pile_is_empty(Pile* pile);
void clear_pile(Pile* pile);
void free_pile(Pile* pile);
void print_pile(Pile* pile, int pile_number);

#endif

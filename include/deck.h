#ifndef DECK_H
#define DECK_H

#include "common.h"

// ============================================
// Clase Card
// ============================================
class Card {
public:
    Card(Color c = Color::RED, int v = 0) : color(c), value(v) {}
    Color getColor() const { return color; }
    int getValue() const { return value; }

private:
    Color color;
    int value;
};

void print_card(const Card &card);

// ============================================
// Clase Deck (RAII)
// ============================================
class Deck {
public:
    Deck();
    void shuffle();
    bool isEmpty() const;
    // Devuelve true si robó, y deja la carta en out
    bool draw(Card &out);
    void print() const;
    int remaining() const { return static_cast<int>(cards.size() - topIndex); }

private:
    std::vector<Card> cards;
    std::size_t topIndex;
};

// ============================================
// Clase Pile (RAII, contenedor temporal)
// ============================================
class Pile {
public:
    Pile();
    bool add(const Card &card);
    bool isFull() const;
    bool isEmpty() const;
    void clear();
    void print(int pileNumber) const;
    bool isTaken() const { return taken; }
    void markTaken() { taken = true; count = 0; }
    int size() const { return count; }
    const std::vector<Card>& getCards() const { return cards; }

private:
    std::vector<Card> cards;
    int count;
    bool full;
    bool taken;
};

#endif

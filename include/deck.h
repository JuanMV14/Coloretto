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
// Clase Deck (con memoria dinámica explícita)
// ============================================
class Deck {
public:
    Deck();
    ~Deck();  // Destructor para liberar memoria
    void initialize();  // Inicializa la baraja (crea cartas, elige color removido)
    void shuffle();
    bool isEmpty() const;
    // Devuelve true si robó, y deja la carta en out
    bool draw(Card &out);
    void print() const;
    int remaining() const { return static_cast<int>(capacity - topIndex); }
    Color getRemovedColor() const { return removedColor; }
    int getTotalCards() const { return static_cast<int>(capacity); }

private:
    Card* cards;        // Array dinámico de cartas (malloc/new)
    std::size_t capacity;  // Capacidad total del array
    std::size_t topIndex;  // Índice de la siguiente carta a robar
    Color removedColor;
    
    void resizeCards(std::size_t newCapacity);  // Redimensiona el array dinámicamente
};

// ============================================
// Clase Pile (con memoria dinámica explícita)
// ============================================
class Pile {
public:
    Pile();
    ~Pile();  // Destructor para liberar memoria
    bool add(const Card &card);
    bool isFull() const;
    bool isEmpty() const;
    void clear();
    void print(int pileNumber) const;
    bool isTaken() const { return taken; }
    void markTaken() { taken = true; count = 0; }
    int size() const { return count; }
    const Card* getCards() const { return cards; }  // Retorna puntero al array
    std::size_t getCardCount() const { return count; }

private:
    Card* cards;        // Array dinámico de cartas (malloc/new)
    std::size_t count;  // Número actual de cartas
    bool full;
    bool taken;
};

#endif
